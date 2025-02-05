#pragma once
#include <functional>

namespace UIBackend
{
	void START(const std::function<void()>& loadCallback, const std::function<void()>& updateCallback);
}
