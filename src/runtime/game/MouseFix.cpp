#include <Windows.h>
#include <imgui.h>
#include "../util/HookingUtil.hpp"

namespace MouseFix
{
	bool isMouseShown = false;

	void ShowMouse(bool toggle)
	{
		ImGui::GetIO().MouseDrawCursor = toggle;
		isMouseShown = toggle;
	}

	$Hook(int, ClipCursorHOOK, (const RECT* lpRect))
	{
		RECT rect;
		GetWindowRect(GetActiveWindow(), &rect);

		return $CallOrig(ClipCursorHOOK, &rect);
	}

	$Hook(void, ProcessTouch, (IL2CPP::Object* _this, bool idk0, bool idk1))
	{
		if (isMouseShown)
		{
			return;
		}

		$CallOrig(ProcessTouch, _this, idk0, idk1);
	}

	$Hook(void, Update, (IL2CPP::Object* _this))
	{
		HWND hwnd = FindWindowA(NULL, "Pixel Gun 3D");
		HWND activeWindow = GetForegroundWindow();

		if (activeWindow == hwnd && !IsIconic(hwnd) && !isMouseShown)
		{
			RECT windowRect;
			GetWindowRect(hwnd, &windowRect);

			if (windowRect.left >= 0 && windowRect.top >= 0)
			{
				POINT center;
				center.x = (windowRect.left + windowRect.right) / 2;
				center.y = (windowRect.top + windowRect.bottom) / 2;

				SetCursorPos(center.x, center.y);
			}
		}

		$CallOrig(Update, _this);
	}

	void INIT()
	{
		using namespace IL2CPP::ClassMapping;

		$RegisterHook(
			ClipCursorHOOK, 
			ClipCursor
		);

		$RegisterHook(
			ProcessTouch,
			GetClass("UICamera")->GetMethod("ProcessTouch"),
		);

		$RegisterHook(
			Update, 
			GetClass("UICamera")->GetMethod("Update"),
		);
	}
}