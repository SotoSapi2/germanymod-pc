#include "UIManager.hpp"

#include <vector>
#include <imgui.h>
#include <Logger.hpp>

#include "UIBackend.hpp"

namespace UIManager
{
    bool uiManagerInitlized = false;
	std::vector<std::function<void()>> uiUpdateCallbacks{};

	void RegisterUIUpdate(std::function<void()> updateCallback)
	{
		uiUpdateCallbacks.push_back(updateCallback);
	}

	void UIUpdateManager()
	{
		if (ImGui::BeginTabBar("main-tab"), ImGuiTabBarFlags_FittingPolicyResizeDown)
		{
			for (std::function<void()> func : uiUpdateCallbacks)
			{
				func();
			}

			ImGui::EndTabBar();
		}
	}

    void INIT()
    {
		if (uiManagerInitlized)
		{
			throw std::runtime_error("UI Manager already initialized.");
		}

		LOG_INFO("UI initializated.");
		UIBackend::INIT(UIUpdateManager);
    }
}