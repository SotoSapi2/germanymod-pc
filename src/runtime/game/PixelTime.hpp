#pragma once
#include <functional>

namespace PixelTime
{
	void RegisterUpdate(std::function<void()> callback);

	void INIT();
}