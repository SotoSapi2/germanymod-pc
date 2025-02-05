#pragma once
#include "../Main.hpp"

namespace IL2CPP
{
	struct String : public Object
	{
		private:
		int length;
		wchar_t chars[32];

		public:
		static String* Create(const char* str);

		static String* Create(const std::string& str);

		bool Equals(String* str);

		bool Equals(const char* str);

		bool Equals(const std::string& str);

		bool Contains(String* str);

		bool Contains(const char* str);

		bool Contains(const std::string& str);

		bool IsNullOrEmpty();

		int GetLength() const;

		std::string ToString();

		//static String* FromArray(Array<char>* obj)
		//{
		//	static Class* encoding = (Class*) IMPORT::il2cpp_class_from_name(
		//		(IMPORT::Il2CppImage*) CommonImages::Corlib,
		//		"System.Text",
		//		"Encoding"
		//	);
		//
		//	static auto GetUTF8 = (void* (*)()) encoding->GetMethod("get_UTF8", 0)->GetPointer();
		//
		//	static auto GetString = (String * (*)(void*, void*)) encoding->GetMethod("GetString", 1)->GetPointer();
		//
		//	return GetString(GetUTF8(), obj);
		//}

		//Array<char>* ToArray()
		//{
		//	static Class* encoding = (Class*) IMPORT::il2cpp_class_from_name(
		//		(IMPORT::Il2CppImage*) CommonImages::Corlib,
		//		"System.Text",
		//		"Encoding"
		//	);
		//
		//	static auto GetUTF8 = (void* (*)()) encoding->GetMethod("get_UTF8", 0)->GetPointer();
		//
		//	static auto GetBytes = (Array<char>*(*)(void*, void*)) encoding->GetMethod("GetBytes", 1)->GetPointer();
		//
		//	return GetBytes(GetUTF8(), this);
		//}
	};
}