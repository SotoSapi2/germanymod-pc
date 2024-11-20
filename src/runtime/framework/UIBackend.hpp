#pragma once
#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")

namespace UIBackend
{
	// handle window creation & destruction
	void CreateHWindow(const char* windowName) noexcept;
	void DestroyHWindow() noexcept;

	// handle device creation & destruction
	bool CreateDevice() noexcept;
	void ResetDevice() noexcept;
	void DestroyDevice() noexcept;

	// handle ImGui creation & destruction
	void CreateImGui() noexcept;
	void DestroyImGui() noexcept;

	bool BeginRender() noexcept;
	void EndRender() noexcept;
}
