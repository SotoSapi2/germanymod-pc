#include "types.hpp"
#include "tabledefs.hpp"
#include <iostream>

#if defined(DO_API_DEFINE)
	#define DO_API(r, n, p) r (*n) p
#elif defined(DO_API_DECLARE)
	#define DO_API(r, n, p)																	\
	{																						\
		void* ptr = GetProcAddress(gameAssemblyHandle, #n);									\
		IMPORT::n = (r (*) p) ptr;															\
		if(ptr == nullptr)																	\
		{																					\
			std::printf("[ERROR] Couldn't resolve '%s' api function export.\n", #n);		\
		}																					\
	}																						\

#else
	#define DO_API(r, n, p) extern r (*n) p
#endif

#ifndef DO_API_NO_RETURN
#define DO_API_NO_RETURN(r, n, p) DO_API(r,n,p)
#endif

#ifndef DO_API_DECLARE
namespace IL2CPP
{
	namespace IMPORT
	{
		#include "api-import-list.hpp"
	}

	void INIT_API_FUNCTIONS(HMODULE gameAssemblyHandle);
}
#else
	using namespace IL2CPP::IMPORT;
	#include "api-import-list.hpp"
#endif
#undef DO_API