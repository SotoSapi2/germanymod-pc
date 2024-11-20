#include <cstdio>
#include <string>

#include <Windows.h>
#include <Psapi.h>
#include <Logger.hpp>
#include <WinReg.hpp>

#include "framework/ConsoleManager.hpp"
#include "framework/FileDialogService.hpp"
#include "framework/UIManager.hpp"
#include "menu/DumperMenu.hpp"
#include "menu/ReverseToolMenu.hpp"
#include "unity/Unity.hpp"


std::wstring GetLoaderPath()
{
	static winreg::RegKey regis{ HKEY_CURRENT_USER, L"SOFTWARE\\" PROJECT_NAME };
	static winreg::RegExpected<std::wstring> loaderPathSubkey = regis.TryGetStringValue(L"LoaderPath");

	if (loaderPathSubkey.IsValid())
	{
		return loaderPathSubkey.GetValue();
	}
	else
	{
		wchar_t filepathBuffer[MAX_PATH];
		GetModuleFileNameExW(GetCurrentProcess(), nullptr, filepathBuffer, std::size(filepathBuffer));

		std::wstring filepath(filepathBuffer);
		size_t pos = filepath.find_last_of(L"\\/");
		return pos != std::string::npos ? filepath.substr(0, pos) : filepath;
	}
}

void InitializeLogger()
{
	std::wstring loaderPath = GetLoaderPath();
	Logger::SetLogfilePath(loaderPath, L"" PROJECT_NAME);
}

DWORD WINAPI MainThread(LPVOID parameter)
{
	ConsoleManager::INIT();
	InitializeLogger();
	FileDialogService::INIT();
	Unity::INIT();
	UIManager::INIT();

	DumperMenu::INIT();
	ReverseToolMenu::INIT();

	return 0;
}

extern "C" __declspec(dllexport)
bool _stdcall DllMain(HMODULE moduleHandle, DWORD reason, LPVOID reserved)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		CreateThread(nullptr, 0, MainThread, nullptr, 0, nullptr);
	}
	return true;
}