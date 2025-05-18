#include "FileDialogService.hpp"

#include <Logger.hpp>
#include <ImageHlp.h>
#include <codecvt>

#include <shobjidl.h>
#pragma comment(lib, "Ole32.lib")

namespace FileDialogService
{
	void INIT()
	{
		HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
		bool comInitialized = SUCCEEDED(hr) && hr != S_FALSE;

		if (!comInitialized)
		{
			hr = CoInitialize(nullptr);
		}

		if (FAILED(hr))
		{
			LOG_ERROR("Failed to bootstrap FileDialogService. (Error: %i)", GetLastError());
		}
	}

	namespace Internal
	{
		std::optional<std::wstring> GetDisplayName(IFileDialog* pFileDialog)
		{
			std::string displayName;
			IShellItem* pItem = NULL;
			if (SUCCEEDED(pFileDialog->GetResult(&pItem)))
			{
				PWSTR pszFolderPath = NULL;
				pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFolderPath);

				CoTaskMemFree(pszFolderPath);
				pItem->Release();

				return { pszFolderPath };
			}

			return std::nullopt;
		}
	}

	std::optional<std::wstring> GetFilepathLoad(const wchar_t* filter)
	{
		wchar_t szFile[MAX_PATH];

		OPENFILENAMEW ofn;
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = nullptr;
		ofn.lpstrFile = szFile;
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = MAX_PATH;
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = nullptr;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = nullptr;
		ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		ShowCursor(true);
		if (!GetOpenFileNameW(&ofn)) return std::nullopt;
		return ofn.lpstrFile;
	}

	std::optional<std::wstring> GetFilepathSave(const wchar_t* filter)
	{
		wchar_t szFile[MAX_PATH];

		OPENFILENAMEW ofn;
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = nullptr;
		ofn.lpstrFile = szFile;
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = MAX_PATH;
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = nullptr;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = nullptr;
		ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		ShowCursor(true);
		if (!GetSaveFileNameW(&ofn)) return std::nullopt;
		return ofn.lpstrFile;
	}

	std::optional<std::wstring> SelectFolder()
	{
		IFileOpenDialog* pFileDialog = NULL;
		if (FAILED(CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_PPV_ARGS(&pFileDialog))))
		{
			return std::nullopt;
		}

		pFileDialog->SetOptions(FOS_PICKFOLDERS);

		ShowCursor(true);
		if (FAILED(pFileDialog->Show(NULL)))
		{
			return std::nullopt;
		}

		auto displayOpt = Internal::GetDisplayName(pFileDialog);
		pFileDialog->Release();

		if (displayOpt.has_value())
		{
			return displayOpt.value();
		}

		return std::nullopt;
	}
}
