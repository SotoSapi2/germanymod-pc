#pragma once
#include "framework/ConsoleManager.hpp"
#include "framework/FileDialogService.hpp"
#include "framework/UIManager.hpp"
#include "unity/Unity.hpp"
#include "game/data/ClassFinder.hpp"
#include "game/data/PointerFunctions.hpp"
#include "game/PixelTime.hpp"
#include "game/MouseFix.hpp"
#include "game/WeaponManager.hpp"
#include "game/WeaponSounds.hpp"
#include "game/Websocket.hpp"

namespace Bootstrap
{
	void INIT()
	{
		ConsoleManager::INIT();
		FileDialogService::INIT();
		Unity::INIT();

		ClassFinder::INIT();
		PointerFunctions::INIT();

		MouseFix::INIT();
		PixelTime::INIT();
		WeaponManager::INIT();
		WeaponSounds::INIT();
		Websocket::INIT();

		UIManager::INIT();
	}
}