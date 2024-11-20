#pragma once
#include <iostream>
#include <vector>

#include "api-functions.hpp"

typedef uintptr_t FieldPtr;
typedef void* MethodPtr;
typedef std::vector<const char*> FieldPattern;

struct SignaturePattern
{
	const char* modifier = nullptr;
	const char* typeName = nullptr;
	const char* name = nullptr;
	std::vector<const char*> paramTypeName;
};

namespace Il2CppUtils
{
    const Il2CppImage* ResolveImage(Il2CppDomain* domain, const char* dll);

    template<typename T>
    T UnboxIl2CppObject(Il2CppObject* obj);

    FieldPtr GetFieldPointer(Il2CppClass* klass, void* instancePtr, const char* name, int indexOffset = 0);

    FieldPtr GetFieldPointerByIndex(Il2CppClass* klass, Il2CppObject* instancePtr, int index);

    template <typename T>
    T GetStaticField(Il2CppClass* klass, const char* fieldName);

    template <typename T>
    T GetStaticFieldByIndex(Il2CppClass* klass, int index);

    void SetStaticFieldByIndex(Il2CppClass* klass, int index, void* value);

    bool CheckContentCount(Il2CppClass* klass, std::pair<uint32_t, uint32_t> fieldCountPair, std::pair<uint32_t, uint32_t> methodCountPair);

    float CheckFieldPattern(Il2CppClass* klass, FieldPattern& pattern);

    std::string GetMethodModifier(uint32_t flags);

    std::string TraceMethod(const SignaturePattern& pattern);

    void WaitForMethodsinitialization(Il2CppClass* klass);

    MethodPtr GetMethodPointer(Il2CppClass* klass, const SignaturePattern& pattern, int indexOffset = 0);

    MethodPtr GetMethodPointerByName(Il2CppClass* klass, const char* name, int sameMethodNameSkip = 0, int indexOffset = 0);

    MethodPtr GetMethodPointerByIndex(Il2CppClass* klass, int index);
}