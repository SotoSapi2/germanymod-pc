#pragma once
#include "types.hpp"
#include "../Main.hpp"

namespace IL2CPP
{
	namespace VM_FUNCTIONS
	{
		namespace Class
		{
			extern bool (*Init)(IMPORT::Il2CppClass* klass);
		}

		void INIT();
	}
}