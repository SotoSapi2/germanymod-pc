#pragma once
#include <memory>
#include <cstring>
#include "Array.hpp"

namespace IL2CPP
{
	template <typename T>
	struct List : public Object
	{
		private:
		Array<T>* items;
		int32_t size;
		int32_t version;
		IMPORT::Il2CppObject* syncRoot;

		public:	
		int32_t GetSize()
		{
			return this->size;
		}

		T Get(int index)
		{	
			if(index >= GetSize())
			{
				throw Exception::IndexOutOfBounds(
					index,
					GetSize()
				);
			}
			
			return items->Get(index);
		}

		void Set(int index, T value)
		{
			if(index >= GetSize())
			{
				throw Exception::IndexOutOfBounds(
					index,
					GetSize()
				);
			}
			
			items->Set(index, value);
		}

		void ForEach(std::function<void(T)> func)
		{
			for (int i = 0; i < GetSize(); ++i)
			{
				func(items->Get(i));
			}
		}

		void ForEach(std::function<void(T, int)> func)
		{
			for (int i = 0; i < GetSize(); ++i)
			{
				func(items->Get(i), i);
			}
		}
	};

}

