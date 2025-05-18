#include "main.hpp"
#include <iostream>
#include <string>

#include <Windows.h>

#include <tlhelp32.h>
#include <winternl.h>
#include <psapi.h>
#include <cstdlib>
#include <Logger.hpp>
#include <WinReg.hpp>
#include "Test.hpp"
#include <HttpService.hpp>

#define GameName "Pixel Gun 3D"
#define ExecutableName "Pixel Gun 3D.exe"
#define SteamGameID "2524890"

std::wstring GetPath(const std::wstring& str)
{
	size_t pos = str.find_last_of(L"\\/");
	if (pos != std::string::npos)
	{
		return str.substr(0, pos);
	}

	return str;
}

void GetLoaderPath(std::wstring& str)
{
	HMODULE hexecutable = GetModuleHandle(nullptr);
	wchar_t rawPath[MAX_PATH];
	GetModuleFileNameW(hexecutable, rawPath, MAX_PATH);

	str = GetPath(rawPath);
}

bool IsExecutableRunning(const char* executableName)
{
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hSnapshot == INVALID_HANDLE_VALUE)
	{
		LOG_INFO("CreateToolhelp32Snapshot fail. (Error: %i)", GetLastError());
		return false;
	}

	if (Process32First(hSnapshot, &pe32))
	{
		do
		{
			if (strcmp(pe32.szExeFile, executableName) == 0)
			{
				return true;
			}
		}
		while (Process32Next(hSnapshot, &pe32));
	}
	else
	{
		LOG_INFO("Process32First fail. (Error: %i)", GetLastError());
		CloseHandle(hSnapshot);
		return false;
	}

	CloseHandle(hSnapshot);
	return false;
}

void WaitForProcess(const char* executableName, DWORD& outProcessId)
{
	while (true)
	{
		PROCESSENTRY32 pe32;
		pe32.dwSize = sizeof(PROCESSENTRY32);
		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

		if (hSnapshot == INVALID_HANDLE_VALUE)
		{
			LOG_INFO("CreateToolhelp32Snapshot fail. (Error: %i)", GetLastError());
			return;
		}

		if (Process32First(hSnapshot, &pe32))
		{
			do
			{
				if (strcmp(pe32.szExeFile, executableName) == 0)
				{
					outProcessId = pe32.th32ProcessID;
					return;
				}
			}
			while (Process32Next(hSnapshot, &pe32));
		}
		else
		{
			LOG_INFO("Process32First fail. (Error: %i)", GetLastError());
			CloseHandle(hSnapshot);
			return;
		}

		CloseHandle(hSnapshot);
	}
}

void WaitForThread(HANDLE* threadHandle, DWORD pid)
{
	while (true)
	{
		THREADENTRY32 threadEntry;
		threadEntry.dwSize = sizeof(threadEntry);
		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);

		if (hSnapshot == INVALID_HANDLE_VALUE)
		{
			LOG_INFO("CreateToolhelp32Snapshot fail. (Error: %i)", GetLastError());
			return;
		}

		if (Thread32First(hSnapshot, &threadEntry))
		{
			do
			{
				if (threadEntry.th32OwnerProcessID == pid)
				{
					*threadHandle = OpenThread(THREAD_SET_CONTEXT, false, threadEntry.th32ThreadID);
					return;
				}
			}
			while (Thread32Next(hSnapshot, &threadEntry));
		}
		else
		{
			LOG_INFO("Thread32First fail. (Error: %i)", GetLastError());
			CloseHandle(hSnapshot);
			return;
		}

		CloseHandle(hSnapshot);
	}
}

std::wstring GetGamePath(HANDLE processHandle)
{
	wchar_t filename[MAX_PATH];
	GetModuleFileNameExW(processHandle, nullptr, filename, MAX_PATH);
	return GetPath(std::wstring(filename));
}

void InjectionProplam()
{
	LOG_INFO(
		"FAILED TO INJECT!!!\n"
		"This error might occurred due to privilege access issue. Try to run the loader as admin and disable anti-virus."
	);
	system("pause");
	exit(GetLastError());
}

void InjectModule(HANDLE processHandle, const std::wstring& modulePath)
{
	static HMODULE kernal32Module = GetModuleHandle("kernel32.dll");
	static FARPROC loadLibraryPtr = GetProcAddress(kernal32Module, "LoadLibraryW");

	std::string filename = std::filesystem::path(modulePath).filename().string();
	if (!std::filesystem::exists(modulePath))
	{
		LOG_ERROR("Could not find %s. Please, include %s in the same directory.", filename.c_str(), filename.c_str());
		system("pause");
		return;
	}

	DWORD gamePid = GetProcessId(processHandle);
	size_t modulePathSize = modulePath.length() * sizeof(wchar_t) + 1;

	LPVOID resultBuffer = VirtualAllocEx(processHandle, 0, modulePathSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (!resultBuffer)
	{
		LOG_ERROR("VirtualAllocEx fail. (Error: %i)", GetLastError());
		InjectionProplam();
	}

	if (!WriteProcessMemory(processHandle, resultBuffer, modulePath.c_str(), modulePathSize, NULL))
	{
		LOG_ERROR("WriteProcessMemory fail. (Error: %i)", GetLastError());
		InjectionProplam();
	}

	DWORD oldprotect;
	if (!VirtualProtectEx(processHandle, resultBuffer, modulePathSize, PAGE_READONLY, &oldprotect))
	{
		LOG_ERROR("VirtualProtectEx fail. (Error: %i)", GetLastError());
		InjectionProplam();
	}

	HANDLE threadHandle;
	WaitForThread(&threadHandle, gamePid);
	if (!QueueUserAPC((PAPCFUNC)loadLibraryPtr, threadHandle, (ULONG_PTR)resultBuffer))
	{
		LOG_ERROR("QueueUserAPC fail. (Error: %i)", GetLastError());
		InjectionProplam();
	};
}

void GuardCriticalAsset(const std::wstring& filepath, const char* filename)
{
	if (!std::filesystem::exists(filepath))
	{
		LOG_ERROR(
			"Could not find %s. Please, include %s in the same directory.",
			filename,
			filename
		);

		system("pause");
		exit(0);
	}
}

//void Hug(Furry* anotherFurry)
//{
//	anotherFurry->m_furScore += 10;
//}

//void PrintFurScore()
//{
//	std::cout << m_name << " fur score is: " << m_furScore;
//}

#include <iostream>
#include <vector>
#include <string>
#include <format>

struct Furry
{
	std::string m_name;
	int m_furScore = 100;

	Furry(const std::string& name) : m_name(name) {}

	void AddFur(int furScore)
	{
		m_furScore += furScore;
	}

	void Print() const
	{
		std::cout << m_name << " has furry score: " << m_furScore << std::endl;
	}
};

#include <iostream>
#include <vector>
#include <string>
#include <format>

struct Furry
{
	std::string m_name;
	int m_furScore = 100;

	Furry(const std::string& name) : m_name(name) {}

	void AddFur(int furScore)
	{
		m_furScore += furScore;
	}

	void Print() const
	{
		std::cout << m_name << " has furry score: " << m_furScore << std::endl;
	}
};

std::vector<Furry*> furryList;

void AddFurryToList()
{
	std::string furryName = std::format("Furry number {}", furryList.size());
	furryList.push_back(new Furry(furryName));
}

int main()
{
	for (int i = 0; i < 10; i++)
	{
		AddFurryToList();
	}

	for (const auto& furry : furryList)
	{
		furry->Print();
		delete furry; // Clean up allocated memory
	}

	furryList.clear(); // Clear the vector after deleting objects

	return 0;
}

int main(int argc, const char* argv[])
{
	std::wstring loaderPath;
	GetLoaderPath(loaderPath);

	Logger::SetLogfilePath(
		Logger::DebugOutputType::Stdout, 
		loaderPath + L"\\Logs.txt",
		std::ios_base::trunc
	);

	//Testing::Start();
	//return 0;

	std::wstring runtimePath = loaderPath + L"\\" RUNTIME_DLL_FILENAME;
	GuardCriticalAsset(runtimePath, RUNTIME_DLL_FILENAME);

	winreg::RegKey key{HKEY_CURRENT_USER, L"SOFTWARE\\" PROJECT_NAME};
	key.SetStringValue(L"LoaderPath", loaderPath);

	LOG_INFO(
		"Keybinds to open the menu:\n"
		"- F1\n"
		"- Fn + F1\n"
		"- Right Ctrl\n"
		"- Right Alt\n"
	);

	if (!IsExecutableRunning(ExecutableName))
	{
		LOG_INFO("Launching %s... (Open manually if stuck)", GameName);
		ShellExecute(0, 0, "steam://rungameid/" SteamGameID, 0, 0, SW_SHOW);
	}
	else
	{
		LOG_INFO("Injecting %s...", GameName);
	}

	DWORD gamePID = 0;
	WaitForProcess(ExecutableName, gamePID);
	HANDLE pgHandle = OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_WRITE, false, gamePID);

	if (pgHandle == nullptr)
	{
		LOG_ERROR("OpenProcess fail. (Error: %i)", GetLastError());
		InjectionProplam();
		return 0;
	}

	#if defined(_DEBUG)
	//DebugActiveProcess(gamePID);
	ShellExecute(0, "open", "vsjitdebugger.exe", std::format("-p {0}", gamePID).c_str(), 0, SW_SHOW);
	while (IsExecutableRunning("vsjitdebugger.exe"))
	{
		Sleep(100);
	}
	#endif

	try
	{
		Sleep(1000);
		InjectModule(pgHandle, runtimePath);
	}
	catch (std::exception& err)
	{
		LOG_ERROR("%s", err.what());
	}
	return 0;
}