#include "Stacktrace.hpp"
#include "../import/PointerFunctions.hpp"

Stacktrace* Stacktrace::New()
{
	using namespace IL2CPP::Wrapper;

	static IL2CPP::Class* stackTrace_class = IL2CPP::ClassMapping::GetClass("StackTrace");
	static Method<void(IL2CPP::Object*)> StackTrace$Ctor = stackTrace_class->GetMethod(0x0);
	auto obj = IL2CPP::Class::Create(stackTrace_class);
	StackTrace$Ctor(obj);
	return (Stacktrace*)obj;
}

int Stacktrace::GetLength()
{
	return frames->GetLength();
}

Strackframe* Stacktrace::GetFrame(size_t index)
{
	return frames->Get(index);
}

void Stacktrace::IterateFrames(std::function<void(Strackframe*)> func)
{
	frames->ForEach(func);
}

IL2CPP::String* Stacktrace::ToString()
{
	using namespace IL2CPP::Wrapper;

	static IL2CPP::Class* stackTrace_class = IL2CPP::ClassMapping::GetClass("StackTrace");
	static Method<IL2CPP::String* (Stacktrace*)> StackTrace$ToString = stackTrace_class->GetMethod(0xd);

	return StackTrace$ToString(this);
}