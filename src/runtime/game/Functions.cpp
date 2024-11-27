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

namespace ContentKeyRegister
{
	int GetKeyRegisterSize(OfferItemType type)
	{
		auto list = ContentKeyRegister::GetRegisterList(ContentKeyRegister::GetInstance(), type);
		return list == nullptr ? 0 : list->size;
	}

	void IterateKeyRegister(OfferItemType type, std::function<void(MonoString* x)> callback)
	{
		auto list = ContentKeyRegister::GetRegisterList(ContentKeyRegister::GetInstance(), type);

		list->foreach([&](MonoString* x)
		{
			callback(x);
		});
	}

	void IterateKeyRegister(OfferItemType type, std::function<void(MonoString* x, int i)> callback)
	{
		auto instance = ContentKeyRegister::GetInstance();
		auto list = ContentKeyRegister::GetRegisterList(ContentKeyRegister::GetInstance(), type);

		list->foreach([&](MonoString* x)
		{
			callback(x, ContentKeyRegister::GetItemIndex(instance, type, x));
		});
	}

	void IterateKeyRegister(size_t from, size_t to, OfferItemType type, std::function<void(MonoString* x)> callback)
	{
		auto list = ContentKeyRegister::GetRegisterList(ContentKeyRegister::GetInstance(), type);
		int maxSize = ContentKeyRegister::GetKeyRegisterSize(type);

		for (size_t i = from; i < min(maxSize, to); i++)
		{
			callback(list->Get(i));
		}
	}

	void IterateKeyRegister(size_t from, size_t to, OfferItemType type, std::function<void(MonoString* x, int i)> callback)
	{
		auto instance = ContentKeyRegister::GetInstance();
		auto list = ContentKeyRegister::GetRegisterList(ContentKeyRegister::GetInstance(), type);
		int maxSize = ContentKeyRegister::GetKeyRegisterSize(type);

		for (size_t i = from; i < min(maxSize, to); i++)
		{
			callback(list->Get(i), ContentKeyRegister::GetItemIndex(instance, type, list->Get(i)));
		}
	}

	bool IsKeyBannable(MonoString* key)
	{
		static const std::vector<const char*> bannedKeys = {
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
		for (const char* v : bannedKeys)
		{
			if (strcmp(key->ToUtf8(), v) == 0)
			{
				return true;
			}
		}

		return false;
	}
}