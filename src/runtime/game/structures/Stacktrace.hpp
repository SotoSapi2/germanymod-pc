#pragma once
#include <IL2CPP.hpp>
#include "../import/Structs.hpp"

struct Stacktrace : public IL2CPP::Object
{
	IL2CPP::Array<Strackframe*>* frames;
	IL2CPP::Array<Stacktrace*>* captured_traces;
	bool debug_info;

	static Stacktrace* New();

	int GetLength();

	Strackframe* GetFrame(size_t index);

	void IterateFrames(std::function<void(Strackframe*)> func);

	IL2CPP::String* ToString();
};
