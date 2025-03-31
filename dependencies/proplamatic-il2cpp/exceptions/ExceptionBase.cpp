#include "ExceptionBase.hpp"

namespace IL2CPP
{
	namespace Exception
	{
		ExceptionBase::ExceptionBase()
		{
			#ifdef IL2CPP_STACKTRACE_IS_SUPPORTED
			stacktrace = std::stacktrace::current();
			#endif
		}

		void ExceptionBase::InitializeException(const char* exceptionName, const std::string& message)
		{
			this->message = message;

			#if defined(IL2CPP_STACKTRACE_IS_SUPPORTED)
			std::printf(
				"[Exception Thrown | %s] %s\n%s\n",
				exceptionName,
				message.c_str(),
				std::to_string(stacktrace).c_str()
			);
			#else
			std::printf(
				"[Exception Thrown | %s] %s\n",
				exceptionName,
				message.c_str()
			);
			#endif
		}

		#ifdef IL2CPP_STACKTRACE_IS_SUPPORTED
		const std::stacktrace* ExceptionBase::GetStacktrace() const
		{
			return &stacktrace;
		}
		#endif

		const char* ExceptionBase::what() const throw()
		{
			return message.c_str();
		}
	}
}