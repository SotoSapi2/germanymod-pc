#pragma once
#include <format>
#include "ExceptionBase.hpp"

namespace IL2CPP
{
	namespace Exception
	{
		class NoSuchClass : public ExceptionBase
		{
			public:
			std::string imagename;
			std::string classname;

			explicit NoSuchClass(const std::string& imagename, const std::string& classname) : ExceptionBase()
			{
				this->imagename = imagename;
				this->classname = classname;

				InitializeException(
					IL2CPP_NAMEOF(NoSuchClass),
					std::format("Couldn't find class named \"{0}\" in {1} image.",
						classname,
						imagename
					)
				);
			}
		};
	}
}