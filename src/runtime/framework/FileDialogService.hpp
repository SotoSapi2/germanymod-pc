#pragma once
#include <Windows.h>
#include <optional>
#include <iostream>
#include <string>

namespace FileDialogService
{
	void INIT();
	std::optional<std::string> GetFilepathLoad(const char* filter = "Select a file\0*.*");
	std::optional<std::string> GetFilepathSave(const char* filter = "Select a file\0*.*");
	std::optional<std::string> SelectFolder();
}