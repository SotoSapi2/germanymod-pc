#pragma once
#include <functional>
#include "import/Structs.hpp"

namespace Global
{
	extern int gPlayerLevel;
	extern int gGlobalTick;
	extern AnalyticsParamater offerwallParam;

	void ExecuteOnGameThread(std::function<void()> callback);

	void RegisterUpdate(std::function<void()> callback);

	void INIT();
}