#pragma once
#include <format>
#include "ExceptionBase.hpp"

namespace IL2CPP
{
	namespace Exception
	{
		class IndexOutOfBounds : public ExceptionBase // too much c#, ik bro
		{
			public:
			size_t pointedIndex;
			size_t containerSize;

			explicit IndexOutOfBounds(size_t pointedIndex, size_t arraySize) : ExceptionBase()
			{
				this->pointedIndex = pointedIndex;
				this->containerSize = arraySize;

				InitializeException(
					IL2CPP_NAMEOF(IndexOutOfBounds),
					std::format("Index was outside of the bounds of an array. (array size: {0} | pointed index: {1})",
						arraySize,
						pointedIndex
					)
				);
			}
		};
	}
}

