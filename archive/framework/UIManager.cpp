#include "UIManager.hpp"
#include <vector>
#include <imgui.h>
#include <Logger.hpp>
#include <Windows.h>

#include "UIBackend.hpp"
#include "fonts/Byte.h"
#include "fonts/FontAwesome.h"

#include "../game/MouseFix.hpp"

namespace UIManager
{
	bool isMenuShown = false;
	bool uiManagerInitlized = false;
	std::vector<std::function<void()>> uiUpdateCallbacks;

	void RegisterUIUpdate(std::function<void()> updateCallback)
	{
		uiUpdateCallbacks.push_back(updateCallback);
	}

	void DrawMenu()
	{
		if (ImGui::IsKeyPressed(ImGuiKey_F1))
		{
			isMenuShown = !isMenuShown;
			MouseFix::ShowMouse(isMenuShown);
		}

		if (isMenuShown)
		{
			ImGui::Begin("ProplamPG");
			for (std::function<void()> func : uiUpdateCallbacks)
			{
				func();
			}
			ImGui::End();
		}
	}

	void UILoad()
	{
		
	}

	void UIUpdateManager()
	{
		if (ImGui::IsKeyPressed(ImGuiKey_F1))
		{
			isMenuShown = !isMenuShown;
			MouseFix::ShowMouse(isMenuShown);
		}
		
		if (isMenuShown) 
		{
			DrawMenu();
		}

	}

	void INIT()
	{
		if (uiManagerInitlized)
		{
			throw std::runtime_error("UI Manager already initialized.");
		}

		LOG_INFO("UI initializated.");
		UIBackend::START(UILoad, UIUpdateManager);
	}
}
