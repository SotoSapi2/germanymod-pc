#pragma once
#include <assembler/X64Assembler.hpp>

namespace MemPatcher
{
	using namespace tulip::hook;
	bool WriteAssemblerToMem(const BaseAssembler& assembler);

	bool Restore(void* address);

	bool ReturnInt(void* address, int32_t val);

	bool ReturnTrue(void* address);

	bool ReturnFalse(void* address);

	bool Nop(void* address);

	// nvm it seems tulip hook doesnt support long address load. 
	// i have to implement it by myself. but, im too lazy xd.
	//bool ReturnFloat(void* address, float val);
}