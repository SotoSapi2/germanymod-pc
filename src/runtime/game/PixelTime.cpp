#include "PixelTime.hpp"
#include <vector>
#include "../util/HookingUtil.hpp"

namespace PixelTime
{
	std::vector<std::function<void()>> updateList;

	void RegisterUpdate(std::function<void()> callback)
	{
		updateList.push_back(callback);
	}

	$Hook(void, PixelTimeUpdate, (IL2CPP::Object* _this))
	{
		for (auto& func : updateList)
		{
			func();
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