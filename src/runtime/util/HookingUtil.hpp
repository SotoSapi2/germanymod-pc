#pragma once

void Detour(void* targetPtr, void* replacementFunc);

template<typename Func>
void AttachHook(void* targetPtr, Func* replacementFunc, Func** origFunc);

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
AttachHook(ptr, Hooks::##name, &HookOrigs::##name)

#define OFFSET(x) (void*)(Vars::base + x)