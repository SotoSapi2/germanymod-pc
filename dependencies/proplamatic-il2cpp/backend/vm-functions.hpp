#pragma once
#include <windows.h>
#include "types.hpp"

namespace IL2CPP::VM_FUNCTIONS
{
	using namespace IL2CPP::IMPORT;
	#define IL2CPP_DECLARE_VM_FUNCTIONS
	#include "vm-functions/import.h"
	#undef IL2CPP_DECLARE_VM_FUNCTIONS
}