#include "Global.hpp"
#include <vector>
#include <stack>
#include "../util/HookingUtil.hpp"
#include "import/PointerFunctions.hpp"

namespace Global
{
	int gPlayerLevel = 0;
	int gGlobalTick = 0;

	AnalyticsParamater offerwallParam = {
			499,
			115,
			28,
			0,
			66,
			0,
			100002,
			false,
			0,
			0,
			false,
			0,
			0,
			false,
			0,
			0,
			0,
			false,
			0,
			0,
			false,
			false,
			false,
			true,
			0
	};


	std::stack<std::function<void()>> gameThreadExecutionStack;
	std::vector<std::function<void()>> updateList;

	void ExecuteOnGameThread(std::function<void()> callback)
	{
		gameThreadExecutionStack.push(callback);
	}

	void RegisterUpdate(std::function<void()> callback)
	{
		updateList.push_back(callback);
	}

	$Hook(void, PixelTimeUpdate, (IL2CPP::Object* _this))
	{
		using namespace IL2CPP::ClassMapping;
		gGlobalTick++;

		static auto expControllerInstance = GetClass("ExperienceController")->GetField(0x1b)->GetValue<IL2CPP::Object*>(nullptr);
		gPlayerLevel = ExperienceController::GetCurrentLevel(expControllerInstance);

		for (auto& func : updateList)
		{
			func();
		}

		while (!gameThreadExecutionStack.empty())
		{
			auto func = gameThreadExecutionStack.top();
			func();
			gameThreadExecutionStack.pop();
		}

		$CallOrig(PixelTimeUpdate, _this);
	}

	void INIT()
	{
		using namespace IL2CPP::ClassMapping;

		$RegisterHook(
			PixelTimeUpdate,
			GetClass("PixelTime")->GetMethod("Update")
		);
	}
}