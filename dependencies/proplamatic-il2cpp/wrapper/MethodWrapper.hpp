#pragma once
#include "../Main.hpp"

namespace IL2CPP
{
	namespace Wrapper
	{
		template <typename T>
		class Method;

		template <typename R, typename... Args>
		class Method<R(Args...)>
		{
			protected:
			void* ptr = nullptr;

			public:
			Method()
			{}

			Method(nullptr_t)
			{}

			Method(MethodInfo* methodInfo)
			{
				this->ptr = methodInfo->GetPointer();
			}

			Method(void* pointer)
			{
				this->ptr = pointer;
			}

			Method(const Method& other)
			{
				this->ptr = other.ptr;
			}

			void* GetPointer() const
			{
				return ptr;
			}

			bool IsNull() const
			{
				return ptr == nullptr;
			}

			Method& operator=(MethodInfo* methodInfo)
			{
				this->ptr = methodInfo->GetPointer();
				return *this;
			}

			Method& operator=(void* pointer)
			{
				this->ptr = pointer;
				return *this;
			}

			Method& operator=(const Method& other)
			{
				this->ptr = other.ptr;
				return *this;
			}

			R operator()(Args... args)
			{
				if (IsNull())
				{
					throw Exception::UnexpectedError();
				}

				return ((R(*)(Args...)) GetPointer()) (std::forward<Args>(args)...);
			}

			explicit operator bool() const 
			{
				return this->ptr != nullptr;
			}
		};
	}
}
