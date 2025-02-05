#pragma once
#include "../Main.hpp"

namespace IL2CPP
{
	struct Type : private IMPORT::Il2CppType
	{
		Class* GetClass() const;

		bool IsPassByReference() const;

		uint32_t GetAttributes() const;

		uint32_t GetTypingType() const;
	};
}