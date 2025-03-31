#include <Logger.hpp>
#include "main.hpp"
#include "test/GdiPlusManager.hpp"
#include "test/TestMemPatcher.hpp"
#include <assembler/X64Assembler.cpp>
#include <functional>

class DetourMetadata
{
	public:
	bool trampolineEnabled = false;
	uint8_t* functionAddress = nullptr;
	uint8_t stolenBytes[15] = {
		0x0, 0x0, 0x0, 0x0, 0x0,
		0x0, 0x0, 0x0, 0x0, 0x0,
		0x0, 0x0, 0x0, 0x0, 0x0
	};
	uint8_t stolenBytesSize = 0;
	uint8_t* allocatedCodecave = nullptr;
	DWORD oldTrampoPageGuardFlags = PAGE_EXECUTE_READ;

	protected:
	DetourMetadata(uint8_t* functionAddress)
	{
		// Skip jump if the pointer is heading to jump vector/table.
		if (functionAddress[0] == 0xE9 && functionAddress[-1] != 0xCC)
		{
			int offsetBuffer;
			memcpy(&offsetBuffer, functionAddress + 1, sizeof(offsetBuffer));

			this->functionAddress = functionAddress + (offsetBuffer + 5);
		}
		else
		{
			this->functionAddress = functionAddress;
		}
	}

	virtual void Initialize()
	{
		if (functionAddress == nullptr)
		{
			throw std::runtime_error("functionAddress argument is null.");
		}

		if (VirtualProtect(functionAddress, std::size(stolenBytes), PAGE_EXECUTE_READWRITE, &oldTrampoPageGuardFlags))
		{
			stolenBytesSize = SpecifyBytesToBeStolen();
			memcpy(stolenBytes, functionAddress, stolenBytesSize);
		}
		else
		{
			throw std::runtime_error("Couldn't edit function page protection.");
		}

		allocatedCodecave = reinterpret_cast<uint8_t*>(AllocatePageNearAddress(this->functionAddress));
	}

	virtual uint8_t SpecifyBytesToBeStolen()
	{
		throw std::runtime_error("Method must be overridden.");
	}

	virtual tulip::hook::BaseAssembler AssemblerJmpInstruction()
	{
		throw std::runtime_error("Method must be overridden.");
	}

	static void* AllocatePageNearAddress(void* address)
	{
		SYSTEM_INFO sysInfo;
		GetSystemInfo(&sysInfo);
		const size_t PAGE_SIZE = sysInfo.dwPageSize;

		uintptr_t startAddr = reinterpret_cast<uintptr_t>(address) & ~(PAGE_SIZE - 1);
		uintptr_t minAddr = min(startAddr - 0x7FFFFF00, (uintptr_t)sysInfo.lpMinimumApplicationAddress);
		uintptr_t maxAddr = max(startAddr + 0x7FFFFF00, (uintptr_t)sysInfo.lpMaximumApplicationAddress);

		uintptr_t startPage = (startAddr - (startAddr % PAGE_SIZE));

		int32_t pageOffset = 1;

		while (true)
		{
			uintptr_t byteOffset = pageOffset * PAGE_SIZE;
			uintptr_t highAddr = startPage + byteOffset;
			uintptr_t lowAddr = (startPage > byteOffset) ? startPage - byteOffset : 0;

			bool needsExit = highAddr > maxAddr && lowAddr < minAddr;

			if (highAddr < maxAddr)
			{
				void* outAddr = VirtualAlloc((void*)highAddr, PAGE_SIZE, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

				if (outAddr != nullptr)
				{
					return outAddr;
				}
			}

			if (lowAddr > minAddr)
			{
				void* outAddr = VirtualAlloc((void*)lowAddr, PAGE_SIZE, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

				if (outAddr != nullptr)
				{
					return outAddr;
				}
			}

			pageOffset++;

			if (needsExit)
			{
				throw std::runtime_error("Failed to allocate page.");
			}
		}

		return nullptr;
	}

	void WriteStolenBytesToAssembler(tulip::hook::BaseAssembler& assembler)
	{
		for (size_t i = 0; i < stolenBytesSize; i++)
		{
			assembler.write8(stolenBytes[i]);
		}
	}

	void AllocateCodecave(const tulip::hook::BaseAssembler& assembler)
	{
		SYSTEM_INFO sysInfo;
		GetSystemInfo(&sysInfo);
		if (assembler.m_buffer.size() > sysInfo.dwPageSize)
		{
			throw std::runtime_error("Assembler size is bigger than allocated page size.");
		}

		if (allocatedCodecave != nullptr)
		{
			memcpy(allocatedCodecave, assembler.m_buffer.data(), assembler.m_buffer.size());
		}
		else
		{
			throw std::runtime_error("Couldn't allocate codecave.");
		}
	}

	public:
	void EnableTrampoline()
	{
		if (trampolineEnabled) return;
		//tulip::hook::BaseAssembler jmpTrampoline = AssemblerJmpInstruction();

		tulip::hook::X64Assembler entryJumpAssembler(reinterpret_cast<uintptr_t>(functionAddress));
		entryJumpAssembler.jmp(reinterpret_cast<uintptr_t>(allocatedCodecave));

		if (stolenBytesSize > entryJumpAssembler.m_buffer.size())
		{
			size_t nopAmount = stolenBytesSize - entryJumpAssembler.m_buffer.size();
			for (size_t i = 0; i < nopAmount; i++)
			{
				entryJumpAssembler.nop();
			}
		}

		DWORD oldProt;
		if (VirtualProtect(functionAddress, entryJumpAssembler.m_buffer.size(), PAGE_EXECUTE_READWRITE, &oldProt))
		{
			memcpy(functionAddress, entryJumpAssembler.m_buffer.data(), entryJumpAssembler.m_buffer.size());
			trampolineEnabled = true;
		}
		else
		{
			throw std::runtime_error("Couldn't edit function page protection.");
		}
	}

	void DisableTrampoline()
	{
		if (trampolineEnabled)
		{
			memcpy(functionAddress, stolenBytes, stolenBytesSize);
			trampolineEnabled = false;
		}
	}

	void RestoreFunctionProtection()
	{
		DWORD oldProt;
		if (!VirtualProtect(functionAddress, std::size(stolenBytes), oldTrampoPageGuardFlags, &oldProt))
		{
			throw std::runtime_error("Couldn't restore function page protection.");
		}
	}

	void FreeCodecave()
	{
		if (allocatedCodecave == nullptr)
		{
			throw std::runtime_error("Trying to deallocate codecave before it got allocated first.");
		}

		VirtualFree(allocatedCodecave, 0, MEM_RELEASE);
	}

	~DetourMetadata()
	{
		if (allocatedCodecave)
		{
			FreeCodecave();
		}

		RestoreFunctionProtection();
		DisableTrampoline();
	}
};

class X64Detour : public DetourMetadata
{
	protected:
	X64Detour(uint8_t* functionAddress) :
		DetourMetadata(functionAddress)
	{
		Initialize();
	}

	uint8_t SpecifyBytesToBeStolen() override
	{
		if (functionAddress[0] == 0x40)
		{
			return 10;
		}

		throw std::logic_error("womp womp");
	}

	public:
	static X64Detour* Create(uint8_t* functionAddress)
	{
		X64Detour* data = new X64Detour(functionAddress);
		return data;
	}

	static X64Detour* Create(void* functionAddress)
	{
		return Create(reinterpret_cast<uint8_t*>(functionAddress));
	}

	void PushGeneralRegister()
	{
		
	}

	void AssembleCodecave(std::function<void(tulip::hook::X64Assembler& assembler)> assemblerCallback)
	{
		tulip::hook::X64Assembler assembler(reinterpret_cast<uintptr_t>(this->allocatedCodecave));
		assemblerCallback(assembler);
		
		assembler.label("jump_back");
		WriteStolenBytesToAssembler(assembler);
		assembler.jmp(reinterpret_cast<uintptr_t>(this->functionAddress + 6));

		AllocateCodecave(assembler);
	}

	void WriteRestoreJmpBack(tulip::hook::X64Assembler& assembler)
	{
		assembler.label("restore");
		assembler.push(RCX);
		assembler.push(RDX);
		assembler.push(R8);

		assembler.sub(RSP, 32);
		assembler.movabs(RCX, reinterpret_cast<uintptr_t>(functionAddress));
		assembler.movabs(RDX, reinterpret_cast<uintptr_t>(stolenBytes));
		assembler.mov(R8, std::size(stolenBytes));
		assembler.call(reinterpret_cast<uintptr_t>(memcpy));
		assembler.add(RSP, 32);

		assembler.pop(R8);
		assembler.pop(RDX);
		assembler.pop(RCX);
	}
};

class X64Trap : public X64Detour
{

};

void Test2()//, void* test2, int test3)
{
	LOG_INFO("Hello world!");
}

namespace Testing
{
	using namespace tulip::hook;

	const char* aString = "Hello world from machine code! im called with %i first format argument.\n";

	void Start()
	{
		RegMem64 m;
		X64Detour* detourData = X64Detour::Create(Test2);
		detourData->AssembleCodecave([&](X64Assembler& a)
		{

		});
		//detourData->WriteTrampoRestore();
		detourData->EnableTrampoline();

		//X64Assembler codecave_assembler(reinterpret_cast<uintptr_t>(detourData->codecave));
		////codecave_assembler.pop(RAX);
		////codecave_assembler.call((uintptr_t)Test2);

		////codecave_assembler.movabs(RCX, (uintptr_t)detourData.functionAddress);
		////codecave_assembler.movabs(RDX, (uintptr_t)detourData.ogBytesBeforeReplace);
		////codecave_assembler.mov(R8, detourData.ogBytesBeforeReplaceSize);
		////codecave_assembler.call((uintptr_t)memcpy);

		//codecave_assembler.jmp(reinterpret_cast<uintptr_t>(Test));
		//detourData->WriteCodecave(codecave_assembler);


		Test2();
		//LOG_INFO("%p", detourData->allocatedCodecave);
		//Test(200, 22);
		//Test();
	}
}