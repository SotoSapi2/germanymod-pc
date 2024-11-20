// pasted from: https://github.com/cazzwastaken/borderless-imgui-window
#include "UIBackend.hpp"
#include <ImGui.h>
#include <backends/imgui_impl_dx9.h>
#include <backends/imgui_impl_win32.h>
#include <Logger.hpp>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND window, UINT message, WPARAM wideParameter, LPARAM longParameter);
LRESULT WINAPI WindowProcess(HWND window, UINT message, WPARAM wideParameter, LPARAM longParameter);

namespace UIBackend
{
	// window size
	int WIDTH = 1000;
	int HEIGHT = 550;

	// when this changes, exit threads
	// and close menu :)
	bool isRunning = true;

	// winapi window vars
	HWND window = nullptr;
	WNDCLASSEX windowClass = {};

	// points for window movement
	POINTS position = {};

	// direct x state vars
	PDIRECT3D9 d3d = nullptr;
	LPDIRECT3DDEVICE9 device = nullptr;
	D3DPRESENT_PARAMETERS presentParameters = {};

	void CreateHWindow(const char* windowName) noexcept
	{
		windowClass.cbSize = sizeof(WNDCLASSEX);
		windowClass.style = CS_CLASSDC;
		windowClass.lpfnWndProc = WindowProcess;
		windowClass.cbClsExtra = 0;
		windowClass.cbWndExtra = 0;
		windowClass.hInstance = GetModuleHandleA(0);
		windowClass.hIcon = 0;
		windowClass.hCursor = 0;
		windowClass.hbrBackground = 0;
		windowClass.lpszMenuName = 0;
		windowClass.lpszClassName = "skidding-tool-class";
		windowClass.hIconSm = 0;

		RegisterClassEx(&windowClass);

		window = CreateWindowExA(
			0,
			"skidding-tool-class",
			windowName,
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			WIDTH,
			HEIGHT,
			0,
			0,
			windowClass.hInstance,
			0
		);

		ShowWindow(window, SW_SHOW);
		UpdateWindow(window);
	}

	void DestroyHWindow() noexcept
	{
		DestroyWindow(window);
		UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
	}

	bool CreateDevice() noexcept
	{
		d3d = Direct3DCreate9(D3D_SDK_VERSION);

		if (!d3d)
			return false;

		ZeroMemory(&presentParameters, sizeof(presentParameters));

		presentParameters.Windowed = TRUE;
		presentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
		presentParameters.BackBufferFormat = D3DFMT_UNKNOWN;
		presentParameters.EnableAutoDepthStencil = TRUE;
		presentParameters.AutoDepthStencilFormat = D3DFMT_D16;
		presentParameters.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

		if (d3d->CreateDevice(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			window,
			D3DCREATE_HARDWARE_VERTEXPROCESSING,
			&presentParameters,
			&device) < 0
		)
		{
			return false;
		}

		return true;
	}

	void ResetDevice() noexcept
	{
		ImGui_ImplDX9_InvalidateDeviceObjects();

		const auto result = device->Reset(&presentParameters);

		if (result == D3DERR_INVALIDCALL)
			IM_ASSERT(0);

		ImGui_ImplDX9_CreateDeviceObjects();
	}

	void DestroyDevice() noexcept
	{
		if (device)
		{
			device->Release();
			device = nullptr;
		}

		if (d3d)
		{
			d3d->Release();
			d3d = nullptr;
		}
	}

	void CreateImGui() noexcept
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ::ImGui::GetIO();

		io.IniFilename = NULL;

		ImGui::StyleColorsDark();

		ImGui_ImplWin32_Init(window);
		ImGui_ImplDX9_Init(device);
	}

	void DestroyImGui() noexcept
	{
		ImGui_ImplDX9_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}

	bool BeginRender() noexcept
	{
		MSG message;
		while (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);

			if (message.message == WM_QUIT)
			{
				isRunning = false;
				return false;
			}
		}

		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		ImGui::SetNextWindowPos({0, 0});
		ImGui::SetNextWindowSize({(float)UIBackend::WIDTH, (float)UIBackend::HEIGHT});
		ImGui::Begin(
			"MainWindow",
			&isRunning,
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoSavedSettings |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoTitleBar
		);

		return true;
	}

	void EndRender() noexcept
	{
		ImGui::End();
		ImGui::EndFrame();

		device->SetRenderState(D3DRS_ZENABLE, FALSE);
		device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		device->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

		device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 255), 1.0f, 0);

		if (device->BeginScene() >= 0)
		{
			ImGui::Render();
			ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
			device->EndScene();
		}

		const auto result = device->Present(0, 0, 0, 0);

		// Handle loss of D3D9 device
		if (result == D3DERR_DEVICELOST && device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
		{
			ResetDevice();
		}
	}
}

LRESULT WINAPI WindowProcess(HWND window, UINT message, WPARAM wideParameter, LPARAM longParameter)
{
	if (ImGui_ImplWin32_WndProcHandler(window, message, wideParameter, longParameter))
		return true;

	switch (message)
	{
		case WM_SIZE:
		{
			if (UIBackend::device && wideParameter != SIZE_MINIMIZED)
			{
				UIBackend::WIDTH = LOWORD(longParameter); // Queue resize
				UIBackend::HEIGHT = HIWORD(longParameter);
				UIBackend::presentParameters.BackBufferWidth = LOWORD(longParameter);
				UIBackend::presentParameters.BackBufferHeight = HIWORD(longParameter);
				UIBackend::ResetDevice();
			}
			return 0;
		}

		case WM_SYSCOMMAND:
		{
			if ((wideParameter & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
				return 0;
			break;
		}

		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		case WM_LBUTTONDOWN:
		{
			UIBackend::position = MAKEPOINTS(longParameter); // set click points
			return 0;
		}

		case WM_MOUSEMOVE:
		{
			if (wideParameter == MK_LBUTTON)
			{
				const auto points = MAKEPOINTS(longParameter);
				auto rect = ::RECT{};

				GetWindowRect(UIBackend::window, &rect);

				rect.left += points.x - UIBackend::position.x;
				rect.top += points.y - UIBackend::position.y;

				if (UIBackend::position.x >= 0 &&
					UIBackend::position.x <= UIBackend::WIDTH &&
					UIBackend::position.y >= 0 && UIBackend::position.y <= 19)
					SetWindowPos(
						UIBackend::window,
						HWND_TOPMOST,
						rect.left,
						rect.top,
						0, 0,
						SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOZORDER
					);
			}
			return 0;
		}
	}

	return DefWindowProc(window, message, wideParameter, longParameter);
}