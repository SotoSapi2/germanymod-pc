#include "String.hpp"
#include <codecvt>

namespace IL2CPP
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;

	String* String::Create(const char* str)
	{
		return (String*)IMPORT::il2cpp_string_new(str);
	}

	String* String::Create(const std::string& str)
	{
		return (String*)IMPORT::il2cpp_string_new(str.c_str());
	}

	bool String::Equals(String* str)
	{
		return this->ToString() == str->ToString();
	}

	bool String::Equals(const char* str)
	{
		return this->ToString() == str;
	}

	bool String::Equals(const std::string& str)
	{
		return this->ToString() == str;
	}

	bool String::Contains(String* str)
	{
		static auto System_String_Contains = (bool (*)(String*, String*)) DefaultTypeClass::String->GetMethod(0x65)->GetPointer();
		return System_String_Contains(this, str);
	}

	bool String::Contains(const char* str)
	{
		return this->ToString().find(str) != std::string::npos;
	}

	bool String::Contains(const std::string& str)
	{
		return this->ToString().find(str) != std::string::npos;
	}

	bool String::IsNullOrEmpty()
	{
		return ToString().empty();
	}

	int String::GetLength() const
	{
		return this->length;
	}

	std::string String::ToString()
	{
		return { converter.to_bytes(this->chars, this->chars + this->length)};
	}
}