#include <Windows.h>
#include "../ClassFinder.hpp"
#include "../../util/HookingUtil.hpp"

namespace MouseFix
{
	bool isMouseShown = true;

	$Hook(int, ShowCursorHOOK, (bool toggle))
	{
		return $CallOrig(ShowCursorHOOK, isMouseShown);
	}

	$Hook(int, ClipCursorHOOK, (const RECT* lpRect))
	{
		RECT rect;
		GetWindowRect(GetActiveWindow(), &rect);

		return $CallOrig(ClipCursorHOOK, &rect);
	}

	$Hook(void, ProcessTouch, (Il2CppObject* _this, bool idk0, bool idk1))
	{
		if (isMouseShown)
		{
			return;
		}

		$CallOrig(ProcessTouch, _this, idk0, idk1);
	}

	void ShowMouse(bool toggle)
	{
		isMouseShown = toggle;
	}

	void INIT()
	{
		$RegisterHook(ShowCursorHOOK, ShowCursor);
		$RegisterHook(ClipCursorHOOK, ClipCursor);
		$RegisterHook(ProcessTouch, Il2CppUtils::GetMethodPointerByName(
			GetClass("UICamera"),
			"ProcessTouch"
		));
	}
}