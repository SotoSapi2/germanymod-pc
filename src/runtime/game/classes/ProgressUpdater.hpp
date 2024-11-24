#pragma once
#include "../PointerWrapper.hpp"
#include "../../unity/Unity.hpp"

namespace ProgressUpdater
{
	extern Pointer<Il2CppObject* ()> GetInstance;

	void INIT();
}