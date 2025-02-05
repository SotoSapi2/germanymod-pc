#pragma once
#include <functional>
#include <imgui_notify.hpp>

namespace UIBackend
{
	void START(const std::function<void()>& loadCallback, const std::function<void()>& updateCallback);
}
