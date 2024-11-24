#pragma once
#include "../../unity/mono-wrapper/MonoList.hpp"
#include "../../unity/Unity.hpp"

namespace WeaponManager
{
	extern Il2CppObject* myPlayerMoveC;
	extern MonoList<Il2CppObject*>* playerMoveCList;

	void INIT();
}