#pragma once
#include <Windows.h>
#include <optional>
#include <iostream>
#include <string>

namespace FileDialogService
{
	void INIT();
	std::optional<std::wstring> GetFilepathLoad(const wchar_t* filter = L"Select a file\0*.*");
	std::optional<std::wstring> GetFilepathSave(const wchar_t* filter = L"Select a file\0*.*");
	std::optional<std::wstring> SelectFolder();
}