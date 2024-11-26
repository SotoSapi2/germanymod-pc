#include "PixelTime.hpp"
#include <vector>
#include "data/ClassFinder.hpp"
#include "../util/HookingUtil.hpp"

namespace PixelTime
{
	std::vector<std::function<void()>> updateList;

	void RegisterUpdate(std::function<void()> callback)
	{
		updateList.push_back(callback);
	}

	$Hook(void, PixelTimeUpdate, (Il2CppObject* _this))
	{
		for (auto& func : updateList)
		{
			func();
		}

		$CallOrig(PixelTimeUpdate, _this);
	}

	void INIT()
	{
		$RegisterHook(PixelTimeUpdate, Il2CppUtils::GetMethodPointerByName(
			GetClass("PixelTime"),
			"Update"
		));
	}
}