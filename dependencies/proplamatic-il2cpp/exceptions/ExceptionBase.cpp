#include "ExceptionBase.hpp"

namespace IL2CPP
{
	namespace Exception
	{
		ExceptionBase::ExceptionBase()
		{
			stacktrace = std::stacktrace::current();
		}

		void ExceptionBase::InitializeException(const char* exceptionName, const std::string& message)
		{
			this->message = message;

			std::printf(
				"[Exception Thrown | %s] %s\n%s\n",
				exceptionName,
				message.c_str(),
				std::to_string(stacktrace).c_str()
			);
		}

		const std::stacktrace* ExceptionBase::GetStacktrace() const
		{
			return &stacktrace;
		}

		const char* ExceptionBase::what() const throw()
		{
			return message.c_str();
		}
	}
}