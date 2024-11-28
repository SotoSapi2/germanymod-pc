#pragma once
#include <functional>

namespace UIBackend
{
	void START(std::function<void()> loadCallback, std::function<void()> updateCallback);
}
