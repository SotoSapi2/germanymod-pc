/*
ImGui used to make "immediate" ui for a flexible workflow, but this is a different story because you (Mod Developer) have
to extensively modify the ui elements to make shit working. If you (UI Programmer) creating hardcoded ImGui widgets will BADLY hurts overall 
developers' productivity and project scalability. So, please, learn how to program a flexible and modular code. Thanks!

tldr; whoever made this ui is a retard.
*/

#include "Menu.hpp"

#include <imgui.h>
#include <iostream>

#include "../framework/UIBackend.hpp"
#include "../framework/fonts/Byte.h"
#include "../framework/fonts/FontAwesome.h"
#include "Websocket.hpp"
#include "MouseFix.hpp"

enum class Head
{
	Gameplay,
	Account,
	Websocket,
	More
};

namespace SubTab
{
	enum class Gameplay
	{
		Main, 
		Other
	};
}

namespace Menu
{
	const ImVec2 childSize = ImVec2(268, 200);
	float alpha = 1.0f;
	float tabAnim = -10;


	bool isMenuShown = false;
	bool bgDim = true;

	template<typename T>
	void DrawSubtab(const char* name, T* var, T tabId)
	{
		const ImVec2 subTabSize = ImVec2(70, 30);
		if (ImGui::SubTab(*var == tabId, name, subTabSize))
		{
			*var = tabId;
		}
	}

	template<typename T>
	void CreateHead(const char* name, const char* icon, T* var, T tabId)
	{
		const ImVec2 tabSize = ImVec2(122, 27);
		if (ImGui::Tab(*var == tabId, icon, name, tabSize))
		{
			*var = tabId;
		}
	}

	void GameplayUpdate()
	{
		using namespace SubTab;
		static Gameplay currentSubtab = Gameplay::Main;

		ImGui::SetCursorPos(ImVec2(150 + 390, 23 + 10));
		DrawSubtab<Gameplay>("Main", &currentSubtab, Gameplay::Main);
		ImGui::SameLine();
		DrawSubtab<Gameplay>("Other", &currentSubtab, Gameplay::Other);

		switch (currentSubtab)
		{
			case Gameplay::Main:
			{
				ImGui::SetCursorPos(ImVec2(150, 101 - tabAnim));
				ImGui::BeginChild("Gameplay", childSize);
				{
					static bool test1 = true;
					static float test2 = 1;

					ImGui::Checkbox("Kill-All", &test1);
					ImGui::SliderFloat("Frost Radius", &test2, 1.0f, 9999.0f);
				}
				ImGui::EndChild();
				break;
			}

			case Gameplay::Other:
			{

				break;
			}
		}
	}

	void AccountUpdate()
	{

	}

	void WebsocketUpdate()
	{
		ImGui::SetCursorPos(ImVec2(150, 101 - tabAnim));
		ImGui::BeginChild("Socket", childSize);
		{
			if (ImGui::Button("Reload Socket", ImVec2(252, 28)))
			{
				Websocket::Reload();
			};
		}
		ImGui::EndChild();
	}

	void MiscUpdate()
	{
		ImGui::SetCursorPos(ImVec2(150, 101 - tabAnim));
		ImGui::BeginChild("Menu", childSize);
		{
			ImGui::Checkbox("Menu Dim", &bgDim);
		}
		ImGui::EndChild();
	}

	void MenuUpdate()
	{
		if (ImGui::IsKeyPressed(ImGuiKey_F1))
		{
			isMenuShown = !isMenuShown;
			MouseFix::ShowMouse(isMenuShown);
		}

		if (!isMenuShown) return;

		ImGui::SetNextWindowBgAlpha(alpha);
		ImGui::SetNextWindowSize(ImVec2(720, 500));
		const auto windowFlag = ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollWithMouse |
								ImGuiWindowFlags_NoDecoration;

		ImGui::Begin("##voidPG", nullptr, windowFlag);
		{
			std::string sub = "08.10.40";

			auto pos = ImVec2{};
			auto size = ImGui::GetWindowSize();
			pos = ImVec2(ImGui::GetWindowPos().x + ImGui::GetStyle().WindowPadding.x, ImGui::GetWindowPos().y + ImGui::GetStyle().WindowPadding.y); //drag

			if (bgDim)
			{
				ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(0, 0), ImVec2(99999999, 99999999), ImColor(0, 0, 0, 100), 0, 0);
			}

			ImGui::GetBackgroundDrawList()->AddShadowRect(ImVec2(pos.x, pos.y), ImVec2(pos.x + 720 - 20, pos.y + 500 - 20), ImColor(130, 215, 130, 255), 100, ImVec2(0, 0), 10);

			ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(pos.x, pos.y), ImVec2(pos.x + 132, pos.y + 480), ImColor(15, 13, 15), 10, ImDrawFlags_RoundCornersLeft);

			ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(pos.x + 132, pos.y), ImVec2(pos.x + 132 + 568, pos.y + 480), ImColor(20, 18, 20), 10, ImDrawFlags_RoundCornersRight);

			ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(pos.x + 132 + 10, pos.y + 10), ImVec2(pos.x + 132 + 10 + 548, pos.y + 10 + 58), ImColor(15, 13, 15), 4);

			ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(pos.x + 132 + 10 + 14, pos.y + 10 + 14), ImVec2(pos.x + 132 + 10 + 14 + 30, pos.y + 10 + 14 + 30), ImColor(255, 255, 255), 3);
			ImGui::GetWindowDrawList()->AddText(ImVec2(pos.x + 132 + 10 + 14 + 30 + 4, pos.y + 10 + 16), ImColor(255, 255, 255), "craze");
			ImGui::GetWindowDrawList()->AddText(ImVec2(pos.x + 132 + 10 + 14 + 30 + 4, pos.y + 10 + 30), ImColor(130, 215, 130), "Expiry:");
			ImGui::GetWindowDrawList()->AddText(ImVec2(pos.x + 132 + 10 + 14 + 30 + 4 + ImGui::CalcTextSize("Expiry: ").x, pos.y + 10 + 30), ImColor(130, 215, 130), sub.c_str());

			ImGui::PushFont(Fonts::Large);
			ImGui::GetWindowDrawList()->AddShadowRect(ImVec2(pos.x + ImGui::CalcTextSize("Bool").x + 15, pos.y + 25 + 10), ImVec2(pos.x + ImGui::CalcTextSize("Bool").x + 30, pos.y + 35), ImColor(255, 255, 255, 120), 40, ImVec2(0, 0), 1, 10);
			ImGui::GetWindowDrawList()->AddText(ImVec2(pos.x + 38, pos.y + 25), ImColor(255, 255, 255), "Bool");

			ImGui::GetWindowDrawList()->AddText(ImVec2(pos.x + 38 + ImGui::CalcTextSize("Bool").x, pos.y + 25), ImColor(130, 215, 130), "PG");
			ImGui::GetWindowDrawList()->AddShadowRect(ImVec2(pos.x + ImGui::CalcTextSize("BoolPG").x + 15, pos.y + 25 + 10), ImVec2(pos.x + ImGui::CalcTextSize("BoolPG").x + 30, pos.y + 35), ImColor(130, 215, 130, 120), 40, ImVec2(0, 0), 1, 10);
			ImGui::PopFont();

			ImGui::SetCursorPos(ImVec2(13, 87));

			static Head selectedHead = Head::Gameplay;
			ImGui::BeginGroup();
			{
				CreateHead<Head>("Gameplay", ICON_FA_GAMEPAD, &selectedHead, Head::Gameplay);
				CreateHead<Head>("Account", ICON_FA_ID_CARD, &selectedHead, Head::Account);
				CreateHead<Head>("Websocket", ICON_FA_CODE, &selectedHead, Head::Websocket);
				CreateHead<Head>("More", ICON_FA_LIST, &selectedHead, Head::More);
			}
			ImGui::EndGroup();

			switch (selectedHead)
			{
				case Head::Gameplay:
					GameplayUpdate();
					break;
				case Head::Account:
					AccountUpdate();
					break;
				case Head::Websocket:
					WebsocketUpdate();
					break;
				case Head::More:
					MiscUpdate();
					break;
			}
		}
		ImGui::End();
	}

	void MenuLoad()
	{
		ImGuiIO& io = ImGui::GetIO();
		//io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange; // dont change cursors

		ImGui::StyleColorsDark();

		ImFontConfig font_config;

		ImFontConfig icons_config;
		icons_config.MergeMode = true;

		static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };

		Fonts::Medium = io.Fonts->AddFontFromMemoryTTF((void*)Medium, sizeof(Medium), 14.f, &font_config);
		io.Fonts->AddFontFromMemoryCompressedTTF((void*)fa6_solid_compressed_data, sizeof(fa6_solid_compressed_size), 14.f, &icons_config, icons_ranges);

		Fonts::Large = io.Fonts->AddFontFromMemoryTTF((void*)Medium, sizeof(Medium), 18.f, &font_config);
	}

	void INIT()
	{
		UIBackend::START(MenuLoad, MenuUpdate);
	}
}