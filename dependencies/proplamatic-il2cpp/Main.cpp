#include "IL2CPP.hpp"

namespace IL2CPP
{
	HMODULE GameAssembly_handle = nullptr;

	Domain* CurrentDomain()
	{
		return (Domain*) IMPORT::il2cpp_domain_get();
	}

	void AttachCurrentThread()
	{
		IMPORT::il2cpp_thread_attach(
			IMPORT::il2cpp_domain_get()
		);
	}

	void INIT()
	{
		static bool il2cppInitialized = false;

		if (il2cppInitialized)
		{
			std::printf("IL2CPP library is already initialized.\n");
			return;
		}

		GameAssembly_handle = GetModuleHandleA("GameAssembly.dll");

		INIT_API_FUNCTIONS(GameAssembly_handle);
		VM_FUNCTIONS::INIT();
		AttachCurrentThread();
		CommonCShrap::INIT();

		il2cppInitialized = true;

		std::printf("IL2CPP library have been initialized.\n");
	}
}