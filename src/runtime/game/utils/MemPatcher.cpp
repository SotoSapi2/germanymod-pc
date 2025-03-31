#include "MemPatcher.hpp"
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

	bool WriteAssamblerToMem(const BaseAssembler& assambler)
	{
		if (assambler.m_buffer.empty())
		{
			return false;
		}

		uint8_t* ptr = new uint8_t[assambler.m_buffer.size()];

		if (!ReadProcessMemory(
			GetCurrentProcess(),
			(void*)assambler.m_baseAddress,
			ptr,
			assambler.m_buffer.size(),
			nullptr
		))
		{
			delete[] ptr;
			return false;
		}

		if (!WriteProcessMemory(
			GetCurrentProcess(),
			(void*)assambler.m_baseAddress,
			assambler.m_buffer.data(),
			assambler.m_buffer.size(),
			nullptr
		))
		{
			delete[] ptr;
			return false;
		}

		memBackupList.try_emplace(assambler.m_baseAddress, ptr, assambler.m_buffer.size());
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

		return WriteAssamblerToMem(assembler);
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

		return WriteAssamblerToMem(assembler);
	}
}