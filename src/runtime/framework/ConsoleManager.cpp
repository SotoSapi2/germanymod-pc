#include "ConsoleManager.hpp"
#include <Windows.h>
#include <cstdio>
#include <Logger.hpp>
#include "../entrypoint.hpp"
#include <io.h>
#include <fcntl.h>

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
		SetConsoleTitle("Logs");
		SetConsoleCtrlHandler(OnConsoleClose, TRUE);
		f1 = freopen("CONIN$", "rb", stdin);
		f2 = freopen("CONOUT$", "wb", stdout);
		f3 = freopen("CONOUT$", "wb", stderr);
	}

	void InitializeLogger()
	{
		Logger::SetLogfilePath(
			Logger::DebugOutputType::Stdout, 
			GetLoaderPath() + L"\\Logs.txt", 
			std::ios_base::app
		);
	}

	void INIT()
	{
		#ifdef _DEBUG
		AllocateConsole();
		#endif
		InitializeLogger();
	}
}