#include "WeaponSounds.hpp"
#include <imgui.h>
#include "../unity/Unity.hpp"
#include "../util/HookingUtil.hpp"
#include "../framework/UIManager.hpp"
#include "../framework/bool.hpp"

namespace WeaponSounds
{

	$Hook(void, WeaponSoundsUpdate, (Il2CppObject* _this))
	{

		if (bools::SilentAim)
		{
			Field<bool>(_this, "isRoundMelee") = true;
			Field<float>(_this, "radiusRoundMelee") = 99999.0f;
		}
		else
		{
			Field<bool>(_this, "isRoundMelee") = false;
			Field<float>(_this, "radiusRoundMelee") = 0.0f;
		}


		if (bools::ExplosiveBullet)
		{
			Field<bool>(_this, "bulletExplode") = true;
		}
		else
		{
			Field<bool>(_this, "bulletExplode") = false;
		}

		
		if (bools::bazooka)
		{
			Field<bool>(_this, "bazooka") = true;
		}
		else
		{
			Field<bool>(_this, "bazooka") = false;
		}


		if (bools::InfiniteAmmo)
		{
			//Field<>(_this, "unknown") = 999;
		}
		else
		{
			//Field<>(_this, "unknown") = 1;
		}


		if (bools::freezer)
		{
			Field<bool>(_this, "freezer") = true;
		}
		else
		{
			Field<bool>(_this, "freezer") = false;
		}

		if (bools::railgun)
		{
			Field<bool>(_this, "railgun") = true;
		}
		else
		{
			Field<bool>(_this, "railgun") = false;
		}

		$CallOrig(WeaponSoundsUpdate, _this);
	}

	void UIUpdate()
	{
		ImGui::Checkbox("Silent Aim", &bools::SilentAim);
		ImGui::Checkbox("Infinite Ammo", &bools::InfiniteAmmo);
		ImGui::Checkbox("Explosive Bullet", &bools::ExplosiveBullet);
		ImGui::Checkbox("Rpg Bullet", &bools::bazooka);
		ImGui::Checkbox("Freezer", &bools::freezer);
		ImGui::Checkbox("Rail Gun", &bools::railgun);

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
