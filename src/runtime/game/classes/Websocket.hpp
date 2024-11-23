#pragma once
#include "../PointerWrapper.hpp"

namespace Websocket
{
	extern Pointer<int(Il2CppObject* instance, MonoString* msgType, Il2CppObject* jsonMsg)> Send;

	void INIT();
}