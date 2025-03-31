#pragma once
#include <assembler/X64Assembler.hpp>

namespace MemPatcher
{
	using namespace tulip::hook;
	bool WriteAssamblerToMem(const BaseAssembler& assambler);

	bool Restore(void* addreas);

	bool ReturnInt(void* addreas, int32_t val);

	bool ReturnTrue(void* addreas);

	bool ReturnFalse(void* addreas);

	bool Nop(void* addreas);
}