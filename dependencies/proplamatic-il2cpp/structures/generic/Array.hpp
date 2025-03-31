#pragma once
#include <functional>
#include "../../Main.hpp"

namespace IL2CPP
{
	template <typename T>
	struct Array : private IMPORT::Il2CppArray
	{
		private:
		T vector[32];

		public:
		static Array* Create(const Class* klass, size_t size)
		{
			if (klass == nullptr)
			{
				throw Exception::NullArgument(
					IL2CPP_NAMEOF(klass)
				);
			}

			return (Array*) IMPORT::il2cpp_array_new(
				(IMPORT::Il2CppClass*) klass, 
				size
			);
		}

		T* GetVectorPointer()
		{
			return vector;
		}

		T Get(size_t index)
		{
			if (index >= this->max_length)
			{
				throw Exception::IndexOutOfBounds(
					index, 
					this->max_length
				);
			}
			return this->vector[index];
		}

		void Set(size_t index, T value)
		{
			if (index >= this->max_length)
			{
				throw Exception::IndexOutOfBounds(
					index,
					this->max_length
				);
			}

			this->vector[index] = value;
		}

		Array* Clone()
		{
			static auto func = (Array<T>* (*)(Array<T>* arr)) DefaultTypeClass::Array->GetMethod(0xf)->GetPointer();
			return func(this);
		}

		size_t GetLength() const
		{
			return this->max_length;
		}

		void ForEach(std::function<void(T)> func)
		{
			for (int i = 0; i < this->max_length; ++i)
			{
				func(this->vector[i]);
			}
		}

		void ForEach(std::function<void(T, size_t)> func)
		{
			for (size_t i = 0; i < this->max_length; ++i)
			{
				func(this->vector[i], i);
			}
		}
	};
}
