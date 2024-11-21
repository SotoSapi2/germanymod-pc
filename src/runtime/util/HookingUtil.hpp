#pragma once

void Detour(void* targetPtr, void* replacementFunc);

void AttachHook(void* targetPtr, void* replacementFunc, void** origFunc);

#define $Hook(returnType, name, param) \
namespace HookOrigs             \
{                               \
    returnType (*name) param;   \
}                               \
namespace Hooks                 \
{                               \
	returnType name param;      \
}                               \
returnType Hooks::##name param  

#define $CallOrig(name, ...)   \
HookOrigs::##name (__VA_ARGS__)

#define $RegisterHook(name, ptr) \
AttachHook(ptr, (void*)Hooks::##name, (void**)&HookOrigs::##name)