#pragma once
#include <stdexcept>
#include <stacktrace>

namespace IL2CPP
{
	namespace Exception
	{
		class ExceptionBase : std::exception
		{
			protected:
			std::stacktrace stacktrace;
			std::string message = "Unspecified error message.";

			ExceptionBase();

			void InitializeException(const char* exceptionName, const std::string& message);

			public:
			const std::stacktrace* GetStacktrace() const;

			const char* what() const throw();
		};
	}
}