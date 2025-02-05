#include <cstdio>
#include <string>

#include <Windows.h>
#include <Psapi.h>
#include <Logger.hpp>
#include <WinReg.hpp>

#include "Bootstrap.hpp"
#include "util/Converter.hpp"

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

DWORD WINAPI MainThread(LPVOID parameter)
{
	Bootstrap::INIT();
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