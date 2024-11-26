#pragma once
#include "../unity/Unity.hpp"

namespace GameplayMain
{
	extern Il2CppObject* myPlayerMoveC;
	extern MonoList<Il2CppObject*>* playerMoveCList;

	void INIT();
}