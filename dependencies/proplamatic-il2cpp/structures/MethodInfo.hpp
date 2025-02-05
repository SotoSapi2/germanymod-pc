#pragma once
#include "../Main.hpp"

namespace IL2CPP
{
	struct MethodInfo : private IMPORT::MethodInfo
	{
		uint32_t GetFlags() const;

		std::string GetFlagsAsString() const;

		uint16_t GetIflags() const;

		Type* GetReturnType() const;

		size_t GetParametersCount() const;

		Type* GetParameterType(size_t index) const;

		std::string GetParameterName(uint32_t index) const;

		Class* GetReturnClass() const;

		std::string GetName() const;

		bool IsGeneric() const;

		void* GetPointer() const;
	};
}
