#include "UIBackend.hpp"
#include <ImGui.h>
#include <backends/imgui_impl_dx10.h>
#include <backends/imgui_impl_dx11.h>
#include <backends/imgui_impl_win32.h>
#include <Logger.hpp>
#include "Kiero.hpp"
#include <IL2CPP.hpp>

#include <d3d11.h>
#include <d3d10.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND window, UINT message, WPARAM wideParameter, LPARAM longParameter);

namespace UIBackend
{
	HWND windowHandle = nullptr;
	WNDPROC orig_WindowProcess = nullptr;
	std::function<void()> updateCallback;
	std::function<void()> loadCallback;

	LRESULT WINAPI WindowProcess(HWND window, UINT message, WPARAM wideParameter, LPARAM longParameter)
	{
		if (ImGui_ImplWin32_WndProcHandler(window, message, wideParameter, longParameter))
		{
			return true;
		}

		return CallWindowProc(orig_WindowProcess, window, message, wideParameter, longParameter);
	}

	HRESULT(WINAPI* orig_Present10)(IDXGISwapChain* pSwapChain, UINT syncInterval, UINT flags);
	HRESULT WINAPI Present10(IDXGISwapChain* pSwapChain, UINT syncInterval, UINT flags)
	{
		static ID3D10Device* device = nullptr;
		static ID3D10RenderTargetView* mainRenderTargetView;
		static bool init = false;

		if (!init)
		{
			if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D10Device), (void**)&device)))
			{
				IL2CPP::AttachCurrentThread();

				DXGI_SWAP_CHAIN_DESC sd;
				pSwapChain->GetDesc(&sd);
				windowHandle = sd.OutputWindow;
				ID3D10Texture2D* pBackBuffer;
				pSwapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), (LPVOID*)&pBackBuffer);
				device->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
				pBackBuffer->Release();
				orig_WindowProcess = (WNDPROC)SetWindowLongPtr(windowHandle, GWLP_WNDPROC, (LONG_PTR)WindowProcess);

				IMGUI_CHECKVERSION();
				ImGui::CreateContext();
				ImGui_ImplWin32_Init(windowHandle);
				ImGui_ImplDX10_Init(device);

				init = true;
			}
			else
			{
				return orig_Present10(pSwapChain, syncInterval, flags);
			}
		}

		ImGui_ImplDX10_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		updateCallback();

		ImGui::Render();

		device->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
		ImGui_ImplDX10_RenderDrawData(ImGui::GetDrawData());

		return orig_Present10(pSwapChain, syncInterval, flags);
	}

	HRESULT(WINAPI* orig_Present11)(IDXGISwapChain* pSwapChain, UINT syncInterval, UINT flags);
	HRESULT Present11(IDXGISwapChain* pSwapChain, UINT syncInterval, UINT flags)
	{
		static ID3D11Device* device = nullptr;
		static ID3D11DeviceContext* context = nullptr;
		static ID3D11RenderTargetView* mainRenderTargetView = nullptr;
		static bool init = false;

		if (!init)
		{
			if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&device)))
			{
				IL2CPP::AttachCurrentThread();

				device->GetImmediateContext(&context);
				DXGI_SWAP_CHAIN_DESC sd;
				pSwapChain->GetDesc(&sd);
				windowHandle = sd.OutputWindow;
				ID3D11Texture2D* pBackBuffer;
				pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
				device->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
				pBackBuffer->Release();
				orig_WindowProcess = (WNDPROC)SetWindowLongPtr(windowHandle, GWLP_WNDPROC, (LONG_PTR)WindowProcess);

				IMGUI_CHECKVERSION();
				ImGui::CreateContext();
				ImGui_ImplWin32_Init(windowHandle);
				ImGui_ImplDX11_Init(device, context);

				loadCallback();

				init = true;
			}
			else
			{
				return orig_Present10(pSwapChain, syncInterval, flags);
			}
		}

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		updateCallback();

		// Notify
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.f);
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(25.f / 255.f, 25.f / 255.f, 25.f / 255.f, 100.f / 255.f));
		ImGui::RenderNotifications();
		ImGui::PopStyleVar(1);
		ImGui::PopStyleColor(1);
		
		ImGui::Render();

		context->OMSetRenderTargets(1, &mainRenderTargetView, nullptr);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		return orig_Present11(pSwapChain, syncInterval, flags);
	}

	void START(const std::function<void()>& loadCallback, const std::function<void()>& updateCallback)
	{
		UIBackend::loadCallback = loadCallback;
		UIBackend::updateCallback = updateCallback;

		if (kiero::init(kiero::RenderType::D3D10) == kiero::Status::Success)
		{
			if (kiero::bind(8, (void**)&orig_Present10, Present10) != kiero::Status::Success)
			{
				LOG_ERROR("Kiero DirectX-10 binding failure.");
			}
		}
		else if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
		{
			if (kiero::bind(8, (void**)&orig_Present11, Present11) != kiero::Status::Success)
			{
				LOG_ERROR("Kiero DirectX-11 binding failure.");
			}
		}
		else
		{
			LOG_ERROR("Kiero initialization failure.");
		}
	}
}
