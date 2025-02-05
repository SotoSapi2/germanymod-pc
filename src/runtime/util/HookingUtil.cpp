#include "HookingUtil.hpp"

#include <Logger.hpp>
#include <Windows.h>
#include <detours.h>

void Detour(void* targetPtr, void* replacementFunc)
{
	if (targetPtr == nullptr)
	{
		LOG_ERROR("Trying hook a null pointer (targetPtr is null)");
		return;
	}

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&targetPtr, replacementFunc);
	DetourTransactionCommit();
}

void AttachHook(void* targetPtr, void* replacementFunc, void** origFunc)
{
	if (targetPtr == nullptr)
	{
		LOG_ERROR("Trying hook a null pointer (targetPtr is null)");
		return;
	}

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&targetPtr, replacementFunc);
	DetourTransactionCommit();

	*origFunc = targetPtr;
}

void AttachHook(const IL2CPP::MethodInfo* method, void* replacementFunc, void** origFunc)
{
	if (method == nullptr)
	{
		LOG_ERROR("Trying hook a null pointer (method is null)");
		return;
	}

	AttachHook(method->GetPointer(), replacementFunc, origFunc);
}