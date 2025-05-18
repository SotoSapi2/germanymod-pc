#pragma once
#include <IL2CPP.hpp>

struct DictionaryBuilder
{
	private:
	IL2CPP::Object* type;
	IL2CPP::Object* instance;

	DictionaryBuilder(IL2CPP::Object* type, IL2CPP::Object* instance);

	public:
	static DictionaryBuilder Create(IL2CPP::String* first, IL2CPP::String* second);

	static DictionaryBuilder Create(const std::string& first, const std::string& second);

	static DictionaryBuilder Create(const IL2CPP::Class* first, const IL2CPP::Class* second);

	//static DictionaryBuilder FromObject(Il2CppObject* obj);

	void Add(IL2CPP::Object* key, IL2CPP::Object* value);

	void Set(IL2CPP::Object* key, IL2CPP::Object* value);

	int Size();

	IL2CPP::Object* Get(IL2CPP::Object* key);

	IL2CPP::Object* GetInstance();
};