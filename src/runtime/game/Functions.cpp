#include "Functions.hpp"

namespace PlayerMoveC
{
	Il2CppObject* GetTransform(Il2CppObject* player)
	{
		Il2CppObject* plrTrans = Field<Il2CppObject*>(player, "myPlayerTransform").Get();

		if (plrTrans == nullptr)
		{
			return nullptr;
		}

		return Component::GetTransform(plrTrans);
	}

	Vector3 GetPosition(Il2CppObject* player)
	{
		if (player == nullptr) return Vector3();
		return Transform::GetPosition(GetTransform(player));
	}

	Il2CppObject* GetSkinName(Il2CppObject* player)
	{
		return Field<Il2CppObject*>(player, "mySkinName").Get();
	}

	Il2CppObject* GetPlayerDamageable(Il2CppObject* player)
	{
		return Field<Il2CppObject*>(player, "mySkinName", -5).Get();
	}

	Il2CppObject* GetPlayerCamera(Il2CppObject* player)
	{
		auto firstPerson = Field<Il2CppObject*>(GetSkinName(player), "firstPersonControl").Get();
		auto cam = Field<Il2CppObject*>(firstPerson, "playerCamera").Get();
		return cam;
	}

	void SetPosition(Il2CppObject* player, Vector3 pos)
	{
		if (player == nullptr);
		Il2CppObject* trans = GetTransform(player);
		Transform::SetPosition(trans, pos);
	}

	bool IsMine(Il2CppObject* player)
	{
		if (player == nullptr) return false;
		return Field<bool>(GetSkinName(player), "isMine").Get();
	}

	bool IsDead(Il2CppObject* player)
	{
		return PlayerDamageable::IsDead(GetPlayerDamageable(player));
	}

	bool IsEnemyTo(Il2CppObject* player, Il2CppObject* target)
	{
		return PlayerDamageable::IsEnemyTo(GetPlayerDamageable(player), target);
	}
}