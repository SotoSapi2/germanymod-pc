#include "UIManager.hpp"

#include <vector>
#include <imgui.h>
#include <Logger.hpp>

#include "UIBackend.hpp"

namespace UIManager
{
    bool uiManagerInitlized = false;
	std::vector<std::function<void()>> uiUpdateFuncs{};

	DWORD WINAPI UIThread(LPVOID parameter)
	{
		UIBackend::CreateHWindow("Skidding-tool");
		UIBackend::CreateDevice();
		UIBackend::CreateImGui();

		while (UIBackend::BeginRender())
		{
			if (ImGui::BeginTabBar("main-tab"), ImGuiTabBarFlags_FittingPolicyResizeDown)
			{
				for (std::function<void()> func : uiUpdateFuncs)
				{
					func();
				}
				ImGui::EndTabBar();
			}
			 
			UIBackend::EndRender();
		}

		UIBackend::DestroyImGui();
		UIBackend::DestroyDevice();
		UIBackend::DestroyHWindow();

		ExitProcess(0);
	}

	void RegisterUIUpdate(std::function<void()> func)
	{
		uiUpdateFuncs.push_back(func);
	}

    void INIT()
    {
		if (uiManagerInitlized)
		{
			throw std::runtime_error("UI Manager already initialized.");
		}

		CreateThread(nullptr, 0, UIThread, nullptr, 0, nullptr);
		LOG_INFO("UI initializated.");
    }
}