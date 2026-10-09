#include "types.hpp"
#include "tabledefs.hpp"
#include <basetsd.h>

#if defined(DO_API_DECLARE)
	#define DO_API(r, n, p) r (*n) p
#elif defined(DO_API_DEFINE)
	#define DO_API(r, n, p)																	\
	{																						\
		void* ptr = GetProcAddress(gameAssemblyHandle, #n);									\
		IMPORT::n = (r (*) p) ptr;															\
		if(ptr == nullptr)																	\
		{																					\
			std::printf("[ERROR] Couldn't resolve '%s' api function export.\n", #n);		\
		}																					\
	}																						
#else
	#define DO_API(r, n, p) extern r (*n) p
#endif

#ifndef DO_API_NO_RETURN
#define DO_API_NO_RETURN(r, n, p) DO_API(r,n,p)
#endif


#ifdef DO_API_DEFINE
#include "api/import.h"
#else
namespace IL2CPP
{
	namespace IMPORT
	{
		#include "api/import.h"
		void INIT_API_FUNCTIONS(HMODULE gameAssemblyHandle);
	}
}
#endif

#undef DO_API