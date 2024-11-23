#include "PointerWrapper.hpp"

#include <Windows.h>
#include "ClassFinder.hpp"

std::vector<PointerWrapper*> PointerWrapperManager::PointerWrapperList;

bool PointerWrapper::Define()
{
	if (GetOffset().has_value())
	{
		static uintptr_t base = (uintptr_t)GetModuleHandle("GameAssembly.dll");
		ptr = reinterpret_cast<MethodPtr>(base + offset);
	}
	else if (GetKlassName().has_value() && GetIndex() > -1)
	{
		ptr = Il2CppUtils::GetMethodPointerByIndex(
			ClassManager::GetRegisteredClass(GetKlassName().value()),
			GetIndex()
		);
	}
	else if (GetKlassName().has_value() && GetMethodName().has_value())
	{
		ptr = Il2CppUtils::GetMethodPointerByName(
			ClassManager::GetRegisteredClass(GetKlassName().value()),
			GetMethodName().value(),
			GetIndexOffset()
		);
	}
	else if (GetKlassName().has_value())
	{
		ptr = Il2CppUtils::GetMethodPointer(
			ClassManager::GetRegisteredClass(GetKlassName().value()),
			GetPattern(),
			GetIndexOffset()
		);
	}

	return ptr != nullptr;
}

void PointerWrapperManager::INIT()
{
	for (auto v : PointerWrapperManager::PointerWrapperList)
	{
		if (!v->Define())
		{
			LOG_ERROR("Failed to define a pointer wrapper.");
		}
	}
}