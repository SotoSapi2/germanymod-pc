#include <Windows.h>
#include "vm-functions.hpp"

namespace IL2CPP
{
	namespace VM_FUNCTIONS
	{
		namespace Class
		{
			bool (*Init)(IMPORT::Il2CppClass* klass);
		}

		void INIT()
		{
			HANDLE currentProcess = GetCurrentProcess();

			int32_t offsetBuffer = 0;
			BYTE* instructionPointer = 0; 

			instructionPointer = (BYTE*)IMPORT::il2cpp_value_box + 1;
			ReadProcessMemory(currentProcess, instructionPointer, &offsetBuffer, sizeof(offsetBuffer), nullptr);
   
			instructionPointer = (BYTE*)IMPORT::il2cpp_value_box + (offsetBuffer + 5);
			ReadProcessMemory(currentProcess, instructionPointer + 17, &offsetBuffer, sizeof(offsetBuffer), nullptr);

			Class::Init = (bool(*)(IMPORT::Il2CppClass*)) (instructionPointer + 16 + (offsetBuffer + 5));
		}
	}
}