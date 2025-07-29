#pragma once
#include "framework/ConsoleManager.hpp"
#include "framework/FileDialogService.hpp"
#include "framework/GdiPlusManager.hpp"
#include "framework/TaskScheduler.hpp"
#include "game/import/ClassFinder.hpp"
#include "game/import/PointerFunctions.hpp"
#include "game/Menu.hpp"
#include "game/Global.hpp"
#include "game/MouseFix.hpp"
#include "game/GameplayMain.hpp"
#include "game/AntiAnalytics.hpp"
#include "game/websocket/WebsocketCore.hpp"

#include <IL2CPP.hpp>

namespace Bootstrap
{
	void ErrorMessageBox()
	{
		ShowWindow(GetActiveWindow(), SW_SHOWMINIMIZED);
		MessageBoxA(
			nullptr,
			"Nazi Mod didn't initialize properly due to pg update.\n"
			"Although still usable, some features may be unusable or crash on use. \n"
			"\n"
			"Please wait for an update for fixes.",
			"Error! ",
			MB_OK | MB_ICONERROR
		);
	}

	void INIT()
	{
		Sleep(2500);

		ConsoleManager::INIT();
		GdiplusManager::INIT();
		FileDialogService::INIT();
		//TaskScheduler::INIT();
		IL2CPP::INIT();

		bool errowShown = false;
		if(!ClassFinder::INIT())
		{
			ErrorMessageBox();
			errowShown = true;
		}

		PointerFunctions::INIT();

		if (gTotalFailedPointerDef > 0 && !errowShown)
		{
			ErrorMessageBox();
		}

		MouseFix::INIT();
		#ifndef NO_FEATURE
		Global::INIT();
		AntiAnalytics::INIT();
		GameplayMain::INIT();
		WebsocketCore::INIT();
		#endif // !NO_FEATURE

		Menu::INIT();
	}
}