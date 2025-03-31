#include "DictionaryBuilder.hpp"
#include "../import/PointerFunctions.hpp"

DictionaryBuilder::DictionaryBuilder(IL2CPP::Object* type, IL2CPP::Object* instance)
{
	this->type = type;
	this->instance = instance;
}

DictionaryBuilder DictionaryBuilder::Create(IL2CPP::String* first, IL2CPP::String* second)
{
	static IL2CPP::Object* dicType = Type::GetType(
		IL2CPP::String::Create("System.Collections.Generic.Dictionary`2")
	);

	auto args = IL2CPP::Array<IL2CPP::Object*>::Create(IL2CPP::DefaultTypeClass::Object, 2);
	args->Set(0, Type::GetType(first));
	args->Set(1, Type::GetType(second));

	IL2CPP::Object* specificDicType = Type::MakeGenericType(dicType, args);
	IL2CPP::Object* instance = Activator::CreateInstance(specificDicType);

	return DictionaryBuilder(specificDicType, instance);
}

DictionaryBuilder DictionaryBuilder::Create(const std::string& first, const std::string& second)
{
	return Create(
		IL2CPP::String::Create(first),
		IL2CPP::String::Create(second)
	);
}

DictionaryBuilder DictionaryBuilder::Create(IL2CPP::Class* first, IL2CPP::Class* second)
{
	return Create(
		IL2CPP::String::Create(first->GetFullName()),
		IL2CPP::String::Create(second->GetFullName())
	);
}

//static DictionaryBuilder DictionaryBuilder::FromObject(Il2CppObject* obj)
//{
//	IL2CPP::Object* dicType = Type::GetObjectType(obj);
//	return DictionaryBuilder(dicType, obj);
//}

void DictionaryBuilder::Add(IL2CPP::Object* key, IL2CPP::Object* value)
{
	IL2CPP::Object* addMethodInfo = Type::GetMethod(this->type, IL2CPP::String::Create("Add"));
	auto args = IL2CPP::Array<IL2CPP::Object*>::Create(IL2CPP::DefaultTypeClass::Object, 2);
	args->Set(0, key);
	args->Set(1, value);

	MethodBase::Invoke(addMethodInfo, this->instance, args);
}

void DictionaryBuilder::Set(IL2CPP::Object* key, IL2CPP::Object* value)
{
	 IL2CPP::Object* addMethodInfo = Type::GetMethod(this->type, IL2CPP::String::Create("set_Item"));
	auto args = IL2CPP::Array<IL2CPP::Object*>::Create(IL2CPP::DefaultTypeClass::Object, 2);
	args->Set(0, key);
	args->Set(1, value);

	MethodBase::Invoke(addMethodInfo, this->instance, args);
}

IL2CPP::Object* DictionaryBuilder::Get(IL2CPP::Object* key)
{
	IL2CPP::Object* addMethodInfo = Type::GetMethod(this->type, IL2CPP::String::Create("get_Item"));
	auto args = IL2CPP::Array<IL2CPP::Object*>::Create(IL2CPP::DefaultTypeClass::Object, 1);
	args->Set(0, key);

	return MethodBase::Invoke(addMethodInfo, this->instance, args);
}

int DictionaryBuilder::Size()
{
	IL2CPP::Object* get_Count = Type::GetMethod(this->type, IL2CPP::String::Create("get_Count"));
	return MethodBase::Invoke(get_Count, this->instance, nullptr)->Unbox<int>();

}

IL2CPP::Object* DictionaryBuilder::GetInstance()
{
	return this->instance;
}