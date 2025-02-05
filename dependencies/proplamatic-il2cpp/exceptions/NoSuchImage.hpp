#pragma once
#include <format>
#include "ExceptionBase.hpp"

namespace IL2CPP
{
	namespace Exception
	{
		class NoSuchImage : public ExceptionBase
		{
			public:
			std::string assemblyname;

			explicit NoSuchImage(const std::string& assemblyname) : ExceptionBase()
			{
				this->assemblyname = assemblyname;

				InitializeException(
					IL2CPP_NAMEOF(NoSuchImage),
					std::format("Couldn't find assembly named \"{0}\".", 
						assemblyname
					)
				);
			}
		};
	}
}