#include "UIManager.hpp"
#include <vector>
#include <imgui.h>
#include <Logger.hpp>
#include <Windows.h>

#include "UIBackend.hpp"
#include "../game/MouseFix.hpp"

namespace UIManager
{
	bool isMenuShown = false;
	bool uiManagerInitlized = false;
	std::vector<std::function<void()>> uiUpdateCallbacks{};

	void RegisterUIUpdate(std::function<void()> updateCallback)
	{
		uiUpdateCallbacks.push_back(updateCallback);
	}

	void ManageTheme()
	{
		ImGuiStyle& style = ImGui::GetStyle();
		ImVec4* colors = style.Colors;

		colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		colors[ImGuiCol_ChildBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.86f, 0.93f, 0.89f, 1.00f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
		colors[ImGuiCol_Button] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
		colors[ImGuiCol_Header] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
		colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
		colors[ImGuiCol_Tab] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		colors[ImGuiCol_TabHovered] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
		colors[ImGuiCol_TabActive] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
		colors[ImGuiCol_PlotLines] = ImVec4(0.86f, 0.93f, 0.89f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.86f, 0.93f, 0.89f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.86f, 0.93f, 0.89f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.86f, 0.93f, 0.89f, 1.00f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(0.86f, 0.93f, 0.89f, 1.00f);
		colors[ImGuiCol_NavHighlight] = ImVec4(0.86f, 0.93f, 0.89f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.86f, 0.93f, 0.89f, 1.00f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);

		style.WindowRounding = 0.0f;
		style.FrameRounding = 0.0f;
		style.GrabRounding = 0.0f;
		style.ScrollbarRounding = 0.0f;
		style.TabRounding = 0.0f;
	}

	void UIUpdateManager()
	{
		ManageTheme();

		if (ImGui::IsKeyPressed(ImGuiKey_F1))
		{
			isMenuShown = !isMenuShown;
			MouseFix::ShowMouse(isMenuShown);
		}
		
		if (isMenuShown) 
		{
			//ImGuiBuilder BTW
			ImGui::SetNextWindowSize({ 694.f,648.f });

			ImGui::Begin("   ");
			ImGui::SetCursorPos({ 12.f,27.f });
			ImGui::PushItemWidth(70.000000);
			ImGui::Text("Proplam-pg");
			ImGui::PopItemWidth();

			ImGui::SetCursorPos({ 25.f,71.f });

			ImGui::BeginChild("c1", { 245.f,450.f }, true);

			for (std::function<void()> func : uiUpdateCallbacks)
			{
				func();
			}
			ImGui::EndChild();
			ImGui::End();
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
