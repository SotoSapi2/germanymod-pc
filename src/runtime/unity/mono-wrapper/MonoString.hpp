#pragma once
#include <memory>
#include <cstring>
#include "../Unity.hpp"
#include "MonoArray.hpp"

struct MonoString : Il2CppObject
{
	public:
	int length;
	Il2CppChar chars[1];

	static MonoString* Create(const char* str)
	{
		return (MonoString*)il2cpp_string_new(str);
	}

	static MonoString* Create(const std::string& str)
	{
		return (MonoString*)il2cpp_string_new(str.c_str());
	}

	bool Equels(MonoString* str)
	{
		static bool* (*func)(MonoString * str_1, MonoString * str_2) = (bool* (*)(MonoString*, MonoString*)) Il2CppUtils::GetMethodPointerByIndex(
			CSharpType::String,
			0x15
		);

		return func(this, str);
	}

	bool Equels(const char* str)
	{
		return this->Equels(Create(str));
	}

	bool Equels(const std::string& str)
	{
		return this->Equels(Create(str));
	}

	bool Contains(MonoString* str)
	{
		static auto func = (bool* (*)(MonoString*, MonoString*)) Il2CppUtils::GetMethodPointerByIndex(
			CSharpType::String,
			0x65
		);

		return func(this, str);
	}

	bool Contains(const char* str)
	{
		return this->Contains(Create(str));
	}

	bool Contains(const std::string& str)
	{
		return this->Contains(Create(str));
	}

	bool IsNullOrEmpty()
	{
		static auto func = (bool* (*)(MonoString*)) Il2CppUtils::GetMethodPointerByIndex(
			CSharpType::String,
			0x9e
		);

		return func(this);
	}

	int GetLength() const
	{
		return this->length;
	}

	const char* ToUtf8()
	{
		static auto GetUTF8 = (void* (*)()) Il2CppUtils::GetMethodPointerByName(
			il2cpp_class_from_name(il2cpp_get_corlib(), "System.Text", "Encoding"),
			"get_UTF8"
		);

		static auto GetBytes = (MonoArray<char>*(*)(void*, MonoString*)) Il2CppUtils::GetMethodPointer(
			il2cpp_class_from_name(il2cpp_get_corlib(), "System.Text", "Encoding"),
			{ "public virtual", "Byte[]", "GetBytes", {"String"} }
		);

		auto arr = GetBytes(GetUTF8(), this);

		std::unique_ptr<char[]> dest(new char[arr->max_length + 1]);
		std::memcpy(dest.get(), arr->vector, arr->max_length);

		dest[arr->max_length] = '\0';

		return dest.release();
	}

	std::string ToUtf8String()
	{
		return std::string(this->ToUtf8());
	}
};