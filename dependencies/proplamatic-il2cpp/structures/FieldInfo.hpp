#pragma once
#include "../Main.hpp"

namespace IL2CPP
{
	struct FieldInfo : private IMPORT::FieldInfo
	{
		int32_t GetOffset() const;

		std::string GetName() const;

		Type* GetType() const;

		int32_t IsStatic() const;

		int32_t IsLiteral() const;

		int32_t GetFlags() const;

		template<typename T>
		T GetValue(Object* instance)
		{
			if (IsStatic() || IsLiteral())
			{
				void* value = nullptr;
				IMPORT::il2cpp_field_static_get_value(this, &value);
				return (T) value;
			}

			if (instance == nullptr)
			{
				throw Exception::NullArgument(
					IL2CPP_NAMEOF(instance)
				);
			}

			return *(T*)((uintptr_t)instance + this->GetOffset());
		}

		template<typename T>
		void SetValue(Object* instance, T value)
		{
			if (IsStatic() || IsLiteral())
			{
				IMPORT::il2cpp_field_static_set_value(this, &value);
				return;
			}

			if (instance == nullptr)
			{
				throw Exception::NullArgument(
					IL2CPP_NAMEOF(instance)
				);
			}

			*(T*)((uintptr_t)instance + this->GetOffset()) = value;
		}
	};
}
