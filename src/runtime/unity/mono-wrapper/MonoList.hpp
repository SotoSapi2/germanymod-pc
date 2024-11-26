#pragma once
#include <memory>
#include <cstring>
#include "../Unity.hpp"
#include "MonoArray.hpp"

template <typename T>
struct MonoList : Il2CppObject
{
	MonoArray<T>* items;
	int size;
	int version;
	void* syncRoot;

	T Get(int index)
	{
		return items->vector[index];
	}

	void Set(T value, int index)
	{
		items->vector[index] = value;
	}

	void foreach(std::function<void(T)> func)
	{
		for (int i = 0; i < size; ++i)
		{
			func(items->vector[i]);
		}
	}

	void foreach(std::function<void(T, int)> func)
	{
		for (int i = 0; i < size; ++i)
		{
			func(items->m_Ivectortems[i], i);
		}
	}
};
