/*
TODO:
- Fix flickering components when you load subtab for the first time. This bug most likey a ImGui bug.
- Put more effort for flexible Button type.
- Rewrite and finish notification system. (imgui_notify.h suck ass)
- Clean up and refactor this choatic mess.
- Hanging every Zack Zuckerberg slaves' on a rope for who contributing inventing React.
*/

#pragma once
#define _USE_MATH_DEFINES
#include <string>
#include <imgui.h>
#include <functional>
#include <memory>
#include <fstream>
#include <optional>
#include <vector>
#include "fonts/IconFontAwesome.h"
#include "UIComponent.hpp"

#undef ERROR // DUMB WINDOWS MACRO :middle_finger:
enum class NotificationType
{
	INFO,
	WARN,
	ERROR
};

struct MenuColorScheme
{
	ImColor bgLow;
	ImColor bgMed;
	ImColor bgHigh;

	ImColor primaryLow;
	ImColor primaryMed;
	ImColor primaryHigh;

	ImColor textMed;
	ImColor textHigh; 
};

namespace Themes
{
	const MenuColorScheme darkBlue =
	{
		ImColor(0, 0, 0),
		ImColor(27, 28, 32),
		ImColor(41, 42, 46),

		ImColor(40, 90, 255),
		ImColor(15, 135, 255),
		ImColor(96, 177, 255),

		ImColor(215, 215, 215),
		ImColor(255, 255, 255)
	};

	const MenuColorScheme darkLavenderBlue =
	{
		ImColor(0, 0, 0),
		ImColor(27, 28, 32),
		ImColor(41, 42, 46),

		ImColor(60, 62, 196),
		ImColor(88, 101, 242),
		ImColor(116, 134, 252),

		ImColor(215, 215, 215),
		ImColor(255, 255, 255)
	};

	const MenuColorScheme darkGreen =
	{
		ImColor(0, 0, 0),
		ImColor(27, 28, 32),
		ImColor(41, 42, 46),

		ImColor(56, 116, 80),
		ImColor(111, 163, 65),
		ImColor(202, 214, 112),

		ImColor(215, 215, 215),
		ImColor(255, 255, 255)
	};
}

namespace UIFramework
{
	namespace Vars
	{
		extern ImFont* gRegularFont;
		extern ImFont* gMediumFont;
		extern ImFont* gLargeFont;
		extern ImFont* gExtraLargeFont;
		extern ImFont* gIconFont;

		extern MenuColorScheme gMenuColorScheme;
	}

	namespace TagService
	{
		std::optional<std::vector< UIComponents::IComponent*>*> GetTaggedComponents(const char* tag);

		void TagComponent(const char* tag, UIComponents::IComponent* component);

		void IterateTaggedComponents(const char* tag, std::function<void(UIComponents::IComponent*)> func);

		void ToggleTagVisibility(const char* tag, bool toggle);
	}

	namespace ConfigManager
	{
		bool LoadConfig(const std::wstring& path);

		bool SaveConfig(const std::wstring& path);
	}

	void DisclaimerWindow(const ImVec2& size, const std::function<void()>& onClose);

	void QueueNotification(NotificationType notifType, const std::string& message);

	void SaveColorScheme(const std::string& path);

	MenuColorScheme ReadColorScheme(const std::string& path);
		
	void INIT(const std::string& assetsPath, MenuColorScheme colorScheme, const std::function<void()>& onUpdateCallback);
}