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
	namespace Global
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
		std::optional<std::vector<IComponent*>*> GetTaggedComponents(const char* tag);

		void TagComponent(const char* tag, IComponent* component);

		void IterateTaggedComponents(const char* tag, std::function<void(IComponent*)> func);

		void ToggleTagVisibility(const char* tag, bool toggle);
	}

	void QueueNotification(NotificationType notifType, const std::string& message);

	void SaveColorScheme(const std::string& path);

	MenuColorScheme ReadColorScheme(const std::string& path);
		
	void INIT(const std::string& assetsPath, MenuColorScheme colorScheme, const std::function<void()>& onUpdateCallback);
}