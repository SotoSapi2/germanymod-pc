#include <Windows.h>
#include <Logger.hpp>

#define DO_API_DEFINE
#include "api-functions.hpp"
#undef DO_API_DEFINE

void import_api()
{
	#define DO_API_DECLARE
	#include "api-functions.hpp"
	#undef DO_API_DECLARE
}

void il2cpp_api_init()
{
	import_api();

	while (!il2cpp_is_vm_thread(nullptr))
	{
		Sleep(1);
	}

	LOG_INFO("Il2cpp api functions initializated.");
}