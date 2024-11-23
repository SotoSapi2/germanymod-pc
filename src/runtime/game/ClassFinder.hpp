#pragma once
#include "../unity/Unity.hpp"

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