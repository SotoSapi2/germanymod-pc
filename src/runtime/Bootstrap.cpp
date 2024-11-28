#pragma once
#include "framework/ConsoleManager.hpp"
#include "framework/FileDialogService.hpp"
#include "unity/Unity.hpp"
#include "game/data/ClassFinder.hpp"
#include "game/data/PointerFunctions.hpp"
#include "game/PixelTime.hpp"
#include "game/MouseFix.hpp"
#include "game/GameplayMain.hpp"
#include "game/WeaponSounds.hpp"
#include "game/Websocket.hpp"
#include "game/Menu.hpp"

namespace Bootstrap
{
	void INIT()
	{
		Sleep(5000);
		ConsoleManager::INIT();
		FileDialogService::INIT();
		Unity::INIT();

		ClassFinder::INIT();
		PointerFunctions::INIT();

		MouseFix::INIT();
		PixelTime::INIT();
		GameplayMain::INIT();
		WeaponSounds::INIT();
		Websocket::INIT();

		Menu::INIT();
		//UIManager::INIT();
	}
}