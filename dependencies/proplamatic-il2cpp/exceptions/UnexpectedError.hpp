#pragma once
#include <format>
#include "ExceptionBase.hpp"

namespace IL2CPP
{
	namespace Exception
	{
		class UnexpectedError : public ExceptionBase
		{
			public:
			explicit UnexpectedError() : ExceptionBase()
			{
				InitializeException(
					IL2CPP_NAMEOF(UnexpectedError),
					"Unexpected error occurred during runtime."
				);
			}
		};
	}
}