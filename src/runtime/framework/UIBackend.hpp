#pragma once
#include <functional>

namespace UIBackend
{
	void INIT(std::function<void()> updateCallback);
}
