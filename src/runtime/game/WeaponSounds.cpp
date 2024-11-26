#include "WeaponSounds.hpp"
#include <imgui.h>
#include "data/ClassFinder.hpp"
#include "util/FieldWrapper.hpp"
#include "../util/HookingUtil.hpp"
#include "../framework/UIManager.hpp"

namespace WeaponSounds
{
	//bools
	bool killAura = false;
	bool silentAim = false;
	bool explosiveBullet = false;
	bool infiniteAmmo = false;
	bool bazooka = false;
	bool freezer = false;
	bool railgun = false;
	bool zoomXray = false;

	//floats

	$Hook(void, WeaponSoundsUpdate, (Il2CppObject* _this))
	{
		if (silentAim)
		{
			Field<bool>(_this, "isRoundMelee") = true;
			Field<float>(_this, "radiusRoundMelee") = 99999.0f;
		}

		if (explosiveBullet)
		{
			Field<bool>(_this, "bulletExplode") = true;
		}

		if (bazooka)
		{
			Field<bool>(_this, "bazooka") = true;
		}

		if (infiniteAmmo)
		{
			//Field<>(_this, "unknown") = 999;
		}

		if (freezer)
		{
			Field<bool>(_this, "freezer") = true;
		}

		if (railgun)
		{
			Field<bool>(_this, "railgun") = true;
		}

		if (zoomXray)
		{
			Field<bool>(_this, "zoomXray") = true;
		}

		$CallOrig(WeaponSoundsUpdate, _this);
	}

	void UIUpdate()
	{
		ImGui::Checkbox("Killaura", &killAura);
		ImGui::Checkbox("Silent Aim", &silentAim);
		ImGui::Checkbox("Infinite Ammo", &infiniteAmmo);
		ImGui::Checkbox("Explosive Bullet", &explosiveBullet);
		ImGui::Checkbox("Rpg Bullet", &bazooka);
		ImGui::Checkbox("Freezer", &freezer);
		ImGui::Checkbox("Rail Gun", &railgun);
		ImGui::Checkbox("Zoom Xray", &zoomXray);
	}

	void INIT()
	{
		$RegisterHook(WeaponSoundsUpdate, Il2CppUtils::GetMethodPointerByName(
			GetClass("WeaponSounds"),
			"Update"
		));

		UIManager::RegisterUIUpdate(UIUpdate);
	}
}