#pragma once
#include "WeaponManager.hpp"
#include "util/FieldWrapper.hpp"
#include "data/ClassFinder.hpp"
#include "../util/HookingUtil.hpp"

namespace WeaponManager
{
	Il2CppObject* myPlayerMoveC = nullptr;
	MonoList<Il2CppObject*>* playerMoveCList = nullptr;

	$Hook(void, WeaponManagerUpdate, (Il2CppObject* _this))
	{
		myPlayerMoveC = Field<Il2CppObject*>(_this, "myPlayerMoveC").Get();
		playerMoveCList = Il2CppUtils::GetStaticFieldByIndex<MonoList<Il2CppObject*>*>(GetClass("PlayerListClass"), 0x0);

		// TODO: Aimbot

		$CallOrig(WeaponManagerUpdate, _this);
	}

	void INIT()
	{
		$RegisterHook(WeaponManagerUpdate, Il2CppUtils::GetMethodPointerByName(
			GetClass("WeaponManager"),
			"Update"
		));
	}
}