#ifdef IL2CPP_DECLARE_VM_FUNCTIONS
namespace Class
{
	extern bool (*Init)(Il2CppClass* klass);
}
#elif defined(IL2CPP_DEFINE_VM_FUNCTIONS)
namespace Class
{
	bool (*Init)(Il2CppClass* klass);
}
#endif

void INIT(HMODULE gameAssemblyHandle);

#if defined(IL2CPP_DEFINE_VM_FUNCTIONS)
	#if defined(UNITY_2023_3)
		#include "2022.3.h"
	#else
		#error Unsupported Unity version or version flag not specified
	#endif
#endif