#pragma once
#include <stdexcept>

#if (defined(_MSVC_LANG) && _MSVC_LANG >= 202004L) || __cplusplus >= 202101L
	#define IL2CPP_STACKTRACE_IS_SUPPORTED
#endif

#ifdef IL2CPP_STACKTRACE_IS_SUPPORTED
	#include <stacktrace>
#endif

namespace IL2CPP
{
	namespace Exception
	{
		class ExceptionBase : std::exception
		{
			protected:
			#ifdef IL2CPP_STACKTRACE_IS_SUPPORTED
			std::stacktrace stacktrace;
			#endif
			std::string message = "Unspecified error message.";

			ExceptionBase();

			void InitializeException(const char* exceptionName, const std::string& message);

			public:
			#ifdef IL2CPP_STACKTRACE_IS_SUPPORTED
			const std::stacktrace* GetStacktrace() const;
			#endif

			const char* what() const throw();
		};
	}
}