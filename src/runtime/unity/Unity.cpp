#include "Unity.hpp"

namespace Unity
{
	void INIT()
	{
		il2cpp_api_init();
		il2cpp_thread_attach(il2cpp_domain_get());
		CommonCShrap::INIT();
	}
}