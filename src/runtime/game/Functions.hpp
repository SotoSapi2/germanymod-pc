#pragma once
#include "import/PointerFunctions.hpp"
#include "import/Structs.hpp"
#include <IL2CPP.hpp>

namespace PlayerMoveC
{
	IL2CPP::Object* GetTransform(IL2CPP::Object* player);

	Vector3 GetPosition(IL2CPP::Object* player);

	IL2CPP::Object* GetSkinName(IL2CPP::Object* player);

	IL2CPP::Object* GetPlayerDamageable(IL2CPP::Object* player);

	IL2CPP::Object* GetPlayerCamera(IL2CPP::Object* player);

	void SetPosition(IL2CPP::Object* player, Vector3 pos);

	bool IsMine(IL2CPP::Object* player);

	bool IsDead(IL2CPP::Object* player);

	bool IsEnemyTo(IL2CPP::Object* player, IL2CPP::Object* target);
}

namespace ContentKeyRegister
{
	int GetKeyRegisterSize(OfferItemType type);

	void IterateKeyRegister(OfferItemType type, std::function<void(IL2CPP::String* x)> callback);

	void IterateKeyRegister(OfferItemType type, std::function<void(IL2CPP::String* x, int i)> callback);

	void IterateKeyRegister(size_t from, size_t to, OfferItemType type, std::function<void(IL2CPP::String* x)> callback);

	bool IsKeyBannable(IL2CPP::String* key);
}