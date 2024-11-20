#pragma once
#include <functional>

namespace UIManager
{
	void RegisterUIUpdate(std::function<void()> func);

	void INIT();
}