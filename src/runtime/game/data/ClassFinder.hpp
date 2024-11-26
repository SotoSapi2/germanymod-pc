#pragma once
#include "../../unity/Unity.hpp"

#undef RegisterClass
#undef GetClass

namespace ClassManager
{
	void RegisterClass(Il2CppClass* klass, const char* name);
	Il2CppClass* GetRegisteredClass(const char* name);
}

Il2CppClass* GetClass(const char* name);

namespace ClassFinder
{
	void INIT();
}