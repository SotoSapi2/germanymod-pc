#include "Functions.hpp"

namespace PlayerMoveC
{
	IL2CPP::Object* GetPlayerTransform(IL2CPP::Object* player)
	{
		IL2CPP::Object* plrTrans = player->GetFieldRef<IL2CPP::Object*>("myPlayerTransform");
		return Component::GetTransform(plrTrans);
	}

	IL2CPP::Object* GetTransform(IL2CPP::Object* player)
	{
		IL2CPP::Object* plrTrans = player->GetFieldRef<IL2CPP::Object*>("myTransform");
		return Component::GetTransform(plrTrans);
	}

	Vector3 GetPosition(IL2CPP::Object* player)
	{
		return Transform::GetPosition(GetPlayerTransform(player));
	}

	IL2CPP::Object* GetSkinName(IL2CPP::Object* player)
	{
		return player->GetFieldPtr<IL2CPP::Object*>("mySkinName");
	}

	IL2CPP::Object* GetPlayerDamageable(IL2CPP::Object* player)
	{
		return player->GetFieldPtr<IL2CPP::Object*>("mySkinName", -5);
	}

	IL2CPP::Object* GetPlayerCamera(IL2CPP::Object* player)
	{
		auto firstPerson = GetSkinName(player)->GetFieldPtr<IL2CPP::Object*>("firstPersonControl");
		auto cam = firstPerson->GetFieldPtr<IL2CPP::Object*>("playerCamera");
		return cam;
	}

	void SetPosition(IL2CPP::Object* player, Vector3 pos)
	{
		IL2CPP::Object* trans = GetPlayerTransform(player);
		Transform::SetPosition(trans, pos);
	}

	bool IsMine(IL2CPP::Object* player)
	{
		return GetSkinName(player)->GetFieldRef<bool>("isMine");
	}

	bool IsDead(IL2CPP::Object* player)
	{
		return PlayerDamageable::IsDead(GetPlayerDamageable(player));
	}

	bool IsEnemyTo(IL2CPP::Object* player, IL2CPP::Object* target)
	{
		return PlayerDamageable::IsEnemyTo(GetPlayerDamageable(player), target);
	}
}

namespace Rocket
{
	void Create(const std::string& prefab, IL2CPP::Object* owner, IL2CPP::Object* ownerWepSounds, bool longLife, Vector3 pos)
	{
		using namespace IL2CPP::Wrapper;
		using namespace IL2CPP::ClassMapping;

		static IL2CPP::Class* rocket_class = GetClass("Rocket");
		static IL2CPP::Class* rocketStack_class = GetClass("RocketStack");

		static Method<IL2CPP::Object* (IL2CPP::Object*)> RocketInit = rocketStack_class->GetMethod(0x1);

		static Method<void(IL2CPP::Object* obj, IL2CPP::Object* owner, IL2CPP::Object* wepSounds,
			IL2CPP::String* wepName, float impulse, float power, int smoke,
			IL2CPP::Object* optional1, int optional2)> SendRocket = rocket_class->GetMethod(0x17);

		IL2CPP::Object* rocketStack = rocketStack_class->GetField(0x0)->GetValue<IL2CPP::Object*>(nullptr);

		IL2CPP::Object* rocketObject = RocketInit(rocketStack);

		if (rocketObject == nullptr)
		{
			return;
		}	

		Transform::SetPosition(GameObject::GetTransform(rocketObject), pos);
		IL2CPP::Object* rocketComponent = GameObject::GetComponent(rocketObject, IL2CPP::String::Create("Rocket"));

		if (longLife)
		{
			GameObject::SetName(rocketObject, IL2CPP::String::Create("ThugHunting"));
		}

		SendRocket(rocketComponent, owner, ownerWepSounds, IL2CPP::String::Create(prefab), 0.0f, 0.0f, 0, nullptr, 0);
	}
}

namespace ContentKeyRegister
{
	int GetKeyRegisterSize(OfferItemType type)
	{
		auto list = ContentKeyRegister::GetRegisterList(ContentKeyRegister::GetInstance(), type);
		return list == nullptr ? 0 : list->GetSize();
	}

	void IterateKeyRegister(OfferItemType type, std::function<void(IL2CPP::String* x)> callback)
	{
		auto list = ContentKeyRegister::GetRegisterList(ContentKeyRegister::GetInstance(), type);

		list->ForEach([&](IL2CPP::String* x)
		{
			callback(x);
		});
	}

	void IterateKeyRegister(OfferItemType type, std::function<void(IL2CPP::String* x, int i)> callback)
	{
		auto instance = ContentKeyRegister::GetInstance();
		auto list = ContentKeyRegister::GetRegisterList(ContentKeyRegister::GetInstance(), type);

		list->ForEach([&](IL2CPP::String* x)
		{
			callback(x, ContentKeyRegister::GetItemIndex(instance, type, x));
		});
	}

	void IterateKeyRegister(size_t from, size_t to, OfferItemType type, std::function<void(IL2CPP::String* x)> callback)
	{
		auto list = ContentKeyRegister::GetRegisterList(ContentKeyRegister::GetInstance(), type);
		int maxSize = ContentKeyRegister::GetKeyRegisterSize(type);

		for (size_t i = from; i < min(maxSize, to); i++)
		{
			callback(list->Get(i));
		}
	}

	void IterateKeyRegister(size_t from, size_t to, OfferItemType type, std::function<void(IL2CPP::String* x, int i)> callback)
	{
		IL2CPP::Object* instance = ContentKeyRegister::GetInstance();
		IL2CPP::List<IL2CPP::String*>* list = ContentKeyRegister::GetRegisterList(ContentKeyRegister::GetInstance(), type);
		int maxSize = ContentKeyRegister::GetKeyRegisterSize(type);

		for (size_t i = from; i < min(maxSize, to); i++)
		{
			callback(list->Get(i), ContentKeyRegister::GetItemIndex(instance, type, list->Get(i)));
		}
	}

	bool IsKeyBannable(const std::string& key)
	{
		static const std::vector<std::string> bannedKeys = {
			"new_well_pistol",
			"fps_destroyer",
			"achieve_hunter",
			"portal_rad_st",
			"portal_ess",
			"Weapon998_wstlnds_fatal_melter",
			"Weapon999_wstlnds_dual_anger",
			"Weapon1000_wstlnds_brutal_thinner",
			"Weapon930_burn_hv_sniper",
			"Weapon929_burn_hand_mgun",
			"Weapon928_burnn_tctcl_knife",
			"Weapon916_plastic_overseer",
			"Weapon917_plastic_instigator",
			"Weapon918_plastic_storm",
			"Weapon1193_freez_prjct_dlp",
			"Weapon1194_freez_prjct_rpd_flock",
			"Weapon1195_freez_prjct_marlin",
			"Weapon1068_psns_dgtl_snrs",
			"Weapon1069_psns_lsr_cclr",
			"Weapon1070_rsns_wv_rlsr",
			"Weapon1001_ddl_lqdtr_x1",
			"Weapon1002_ddl_elmntr_x2",
			"Weapon1003_ddl_prsr_x3",
			"avatar_rgb_hero",
			"iron_sword",
			"hound_weapon_one",
			"desert_fighter",
			"locator",
			"advanced_scout_rifle_l",
			"harsh_punisher_l",
			"manual_inferno_l",
			"offensive_rocket_system_l",
			"headhunter_pistols_l",
			"gadget_class_bullet_medkit",
			"gadget_class_bullet_phonixblood",
			"gadget_class_knight_magnet",
			"gadget_class_valk_jetpack",
			"gadget_class_valk_cyberbees",
			"gadget_class_lifeline_medstation",
			"gadget_class_lifeline_biotiksheald",
			"gadget_class_sara_hook",
			"gadget_class_sara_xray",
			"gadget_class_saber_shurikens",
			"gadget_class_saber_reflector",
			"gadget_class_mex_turret",
			"gadget_class_mex_nucleargrenade",
			"gadget_class_viktor_smilemine",
			"gadget_class_viktor_nutcracker",
			"gadget_class_saber_stealth",
			"gadget_br_fraggrenade",
			"gadget_br_molotov",
			"gadget_br_mine",
			"gadget_mh_mech1",
			"gadget_mh_mech2",
			"gadget_mh_mech3",
			"gadget_hook_monster",
		};

		// ik this is a dumb check but this barely affect overall performance anyways
		for (const std::string& v : bannedKeys)
		{
			if (key == v)
			{
				return true;
			}
		}

		return false;
	}

	bool IsKeyBannable(IL2CPP::String* key)
	{
		return IsKeyBannable(key->ToString());
	}
}