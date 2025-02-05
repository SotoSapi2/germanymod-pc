#pragma once
#include "framework/ConsoleManager.hpp"
#include "framework/FileDialogService.hpp"
#include "game/import/ClassFinder.hpp"
#include "game/import/PointerFunctions.hpp"
#include "game/Menu.hpp"
#include "game/PixelTime.hpp"
#include "game/MouseFix.hpp"
#include "game/GameplayMain.hpp"
#include "game/websocket/WebsocketCore.hpp"

#include <IL2CPP.hpp>

namespace Bootstrap
{
	void INIT()
	{
		Sleep(1000);
		ConsoleManager::INIT();
		FileDialogService::INIT();
		IL2CPP::INIT();

		ClassFinder::INIT();
		PointerFunctions::INIT();

		MouseFix::INIT();
		PixelTime::INIT();
		GameplayMain::INIT();
		WebsocketCore::INIT();

		Menu::INIT();
	}
}