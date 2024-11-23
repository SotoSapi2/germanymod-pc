#include "WeaponSounds.hpp"
#include <imgui.h>
#include "../ClassFinder.hpp"
#include "../../util/HookingUtil.hpp"
#include "../../framework/UIManager.hpp"

namespace WeaponSounds
{
	bool killAura = false;
	bool silentAim = false;
	bool explosiveBullet = false;
	bool infiniteAmmo = false;
	bool bazooka = false;
	bool freezer = false;
	bool railgun = false;

	$Hook(void, WeaponSoundsUpdate, (Il2CppObject* _this))
	{
		if (killAura)
		{
			Field<bool>(_this, "isRoundMelee") = true;
			Field<float>(_this, "radiusRoundMelee") = 99999.0f;
		}

		$CallOrig(WeaponSoundsUpdate, _this);
	}

	void UIUpdate()
	{
		ImGui::Checkbox("Killaura", &killAura);
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