#pragma once
#include <format>
#include "ExceptionBase.hpp"

namespace IL2CPP
{
	namespace Exception
	{
		class NoSuchResult : public ExceptionBase
		{
			public:
			explicit NoSuchResult() : ExceptionBase()
			{
				InitializeException(
					IL2CPP_NAMEOF(NoSuchResult),
					"No Such Result."
				);
			}
		};
	}
}