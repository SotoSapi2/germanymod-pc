#pragma once
#include <functional>
#include <imgui.h>

namespace UIManager
{
	void RegisterUIUpdate(std::function<void()> func);

	void INIT();
}