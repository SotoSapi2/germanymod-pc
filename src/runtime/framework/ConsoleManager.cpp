#include "ConsoleManager.hpp"
#include <Windows.h>
#include <cstdio>
#include <Logger.hpp>

namespace ConsoleManager
{
	FILE* f1 = NULL, * f2 = NULL, * f3 = NULL;

	BOOL WINAPI OnConsoleClose(DWORD dwCtrlType)
	{
		ExitProcess(0);
		return TRUE;
	}

	void AllocateConsole()
	{
		AllocConsole();
		//SetConsoleCP(CP_UTF8);
		//SetConsoleOutputCP(936);
		SetConsoleTitle("Skidding-tool Logs");
		SetConsoleCtrlHandler(OnConsoleClose, TRUE);
		f1 = freopen("CONIN$", "rb", stdin);
		f2 = freopen("CONOUT$", "wb", stdout);
		f3 = freopen("CONOUT$", "wb", stderr);
	}

	void INIT()
	{
		AllocateConsole();
	}
}