#include "ProgressUpdater.hpp"

namespace ProgressUpdater
{
	Pointer<Il2CppObject* ()> GetInstance(
		"ProgressUpdater",
		{"internal", "Void", nullptr, {"Dictionary`2", "Int32"}},
		5
	);

	void INIT()
	{
		// TODO: idk
	}
}