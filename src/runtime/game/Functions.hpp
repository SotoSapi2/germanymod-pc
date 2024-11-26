#pragma once
#include "util/FieldWrapper.hpp"
#include "data/ClassFinder.hpp"
#include "data/PointerFunctions.hpp"
#include "data/Structs.hpp"

namespace PlayerMoveC
{
	Il2CppObject* GetTransform(Il2CppObject* player);

	Vector3 GetPosition(Il2CppObject* player);

	Il2CppObject* GetSkinName(Il2CppObject* player);

	Il2CppObject* GetPlayerDamageable(Il2CppObject* player);

	Il2CppObject* GetPlayerCamera(Il2CppObject* player);

	void SetPosition(Il2CppObject* player, Vector3 pos);

	bool IsMine(Il2CppObject* player);

	bool IsDead(Il2CppObject* player);

	bool IsEnemyTo(Il2CppObject* player, Il2CppObject* target);
}