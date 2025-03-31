#include "TestMemPatcher.hpp"
#include <Windows.h>
#include <unordered_map>
#include <memory>

namespace MemPatcher
{
	using namespace tulip::hook;
	class MemBackup
	{
		public:
		uint8_t* ptr;
		size_t size;

		MemBackup(uint8_t* ptr, size_t size): 
			ptr(ptr),
			size(size)
		{}

		~MemBackup()
		{
			delete[] ptr;
		}
	};

	std::unordered_map<uint64_t, MemBackup> memBackupList;

	bool WriteAssemblerToMem(const BaseAssembler& assembler)
	{
		if (assembler.m_buffer.empty())
		{
			return false;
		}

		uint8_t* ptr = new uint8_t[assembler.m_buffer.size()];

		if (!ReadProcessMemory(
			GetCurrentProcess(),
			(void*)assembler.m_baseAddress,
			ptr,
			assembler.m_buffer.size(),
			nullptr
		))
		{
			delete[] ptr;
			return false;
		}

		if (!WriteProcessMemory(
			GetCurrentProcess(),
			(void*)assembler.m_baseAddress,
			assembler.m_buffer.data(),
			assembler.m_buffer.size(),
			nullptr
		))
		{
			delete[] ptr;
			return false;
		}

		memBackupList.try_emplace(assembler.m_baseAddress, ptr, assembler.m_buffer.size());
		return true;
	}

	bool Restore(void* address)
	{
		if (address == nullptr)
		{
			return false;
		}

		auto it = memBackupList.find((uint64_t)address);
		if (it == memBackupList.end())
		{
			return false;
		}

		bool success = WriteProcessMemory(
			GetCurrentProcess(),
			address,
			it->second.ptr,
			it->second.size,
			nullptr
		);

		memBackupList.erase(it);
		return success;
	}

	bool ReturnInt(void* address, int32_t val)
	{
		if (address == nullptr)
		{
			return false;
		}

		X86Assembler assembler((uint64_t)address);
		assembler.mov(X86Register::EAX, val);
		assembler.ret();

		return WriteAssemblerToMem(assembler);
	}

	bool ReturnTrue(void* address)
	{
		return ReturnInt(address, 1);
	}

	bool ReturnFalse(void* address)
	{
		return ReturnInt(address, 0);
	}

	bool Nop(void* address)
	{
		if (address == nullptr)
		{
			return false;
		}

		X86Assembler assembler((uint64_t)address);
		assembler.ret();

		return WriteAssemblerToMem(assembler);
	}

	// nvm it seems tulip hook doesnt support long/far address load. 
	// i have to implement it by myself. but, im too lazy xd.
	//bool ReturnFloat(void* address, float val)
	//{
	//	if (address == nullptr)
	//	{
	//		return false;
	//	}
	//
	//	RegMem32 m;
	//	X86Assembler assembler(((uint64_t)address));
	//	assembler.push(X86Register::EAX);
	//	assembler.mov(X86Register::EAX, (int64_t)std::addressof(val));
	//	assembler.movss(X86Register::XMM0, m[X86Register::EAX]);
	//	assembler.pop(X86Register::EAX);
	//	assembler.ret();
	//
	//	return WriteAssemblerToMem(assembler);
	//}
}