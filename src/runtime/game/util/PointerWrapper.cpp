#include "PointerWrapper.hpp"

bool PointerWrapper::Define()
{
	if (GetOffset().has_value())
	{
		ptr = reinterpret_cast<MethodPtr>(
			(uintptr_t)GetModuleHandle("GameAssembly.dll") + GetOffset().value()
		);
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