#pragma once
#include <functional>
#include "../Unity.hpp"

template <typename T>
struct MonoArray : Il2CppArray
{
	public:
	T vector[32];

	template<typename T>
	static MonoArray<T>* Create(Il2CppClass* klass, size_t size)
	{
		return (MonoArray<T>*) il2cpp_array_new(klass, size);;
	}

	MonoArray<T>* clone()
	{
		static auto func = (Il2CppObject * (*)(Il2CppObject * arr)) Il2CppUtils::GetMethodPointerByIndex(
			CSharpType::Array,
			0xf
		);

		return func(this);
	}

	template<typename T>
	size_t GetLength() const
	{
		return this->max_length;
	}

	void foreach(std::function<void(T)> func)
	{
		for (int i = 0; i < this->max_length; ++i)
		{
			func(this->vector[i]);
		}
	}

	void foreach(std::function<void(T, size_t)> func)
	{
		for (size_t i = 0; i < this->max_length; ++i)
		{
			func(this->vector[i], size_t);
		}
	}
};

