#pragma once
#include <Logger.hpp>
#include "Unity.hpp"

template <typename T>
class Field
{
	private:
	FieldPtr pointer;

	public:
	Field(FieldPtr fieldPtr) 
	{
		pointer = fieldPtr;
		LOG_ERROR_IF(PointerIsNull(), "Cannot find field (filedPtr is null)");
	}

	Field(Il2CppClass* klass, Il2CppObject* instancePtr, const char* name, int indexOffset = 0)
	{
		pointer = Il2CppUtils::GetFieldPointer(klass, instancePtr, name, indexOffset);
		LOG_ERROR_IF(PointerIsNull(), "Cannot find field");
	}

	Field(Il2CppObject* obj, const char* name, int indexOffset = 0)
	{
		pointer = Il2CppUtils::GetFieldPointer(il2cpp_object_get_class(obj), obj, name, indexOffset);
		LOG_ERROR_IF(PointerIsNull(), "Cannot find field");
	}

	Field(Il2CppClass* klass, Il2CppObject* instancePtr, int index)
	{
		pointer = Il2CppUtils::GetFieldPointerByIndex(klass, instancePtr, index);
		LOG_ERROR_IF(PointerIsNull(), "Cannot find field");
	}

	Field(Il2CppObject* obj, int index)
	{
		pointer = Il2CppUtils::GetFieldPointerByIndex(il2cpp_object_get_class(obj), obj, index);
		LOG_ERROR_IF(PointerIsNull(), "Cannot find field");
	}

	inline T Get()
	{
		LOG_ERROR_IF(PointerIsNull(), "Cannot dereference a null pointer.");
		return *(T*)pointer;
	}

	inline void operator=(T value)
	{
		if (!PointerIsNull())
		{
			*(T*)pointer = value;
		}
		else
		{
			LOG_ERROR("Cannot dereference or assign value to null pointer.");
		}
	}

	inline bool PointerIsNull()
	{
		return pointer == 0;
	}

	inline FieldPtr GetPointer()
	{
		return pointer;
	}
};