#pragma once
#include "../Main.hpp"
#include <type_traits>

namespace IL2CPP
{
	struct Object : private IMPORT::Il2CppObject
	{
		Class* GetClass() const;

		void* GetFieldPtr(const std::string& fieldName, int indexOffset = 0);

		void* GetFieldPtr(size_t index);

		template<typename T>
		inline T& GetFieldRef(const std::string& fieldName, int indexOffset = 0)
		{
			return *(T*) GetFieldPtr(fieldName, indexOffset);
		}

		template<typename T>
		inline T& GetFieldRef(size_t index)
		{
			return *(T*) GetFieldPtr(index);
		}

		template<typename T>
		inline const T& GetFieldRef(const std::string& fieldName, int indexOffset = 0) const
		{
			return *(T*)GetFieldPtr(fieldName, indexOffset);
		}

		template<typename T>
		inline const T& GetFieldRef(size_t index) const
		{
			return *(T*)GetFieldPtr(index);
		}

		template<typename T>
		inline T GetFieldPtr(const std::string& fieldName, int indexOffset = 0)
		{
			return *(T*)GetFieldPtr(fieldName, indexOffset);
		}

		template<typename T>
		inline T GetFieldPtr(size_t index)
		{
			return *(T*)GetFieldPtr(index);
		}

		template<typename T>
		inline const T* GetFieldPtr(const std::string& fieldName, int indexOffset = 0) const
		{
			return (T*)GetFieldPtr(fieldName, indexOffset);
		}

		template<typename T>
		inline const T* GetFieldPtr(size_t index) const
		{
			return (T*)GetFieldPtr(index);
		}

		template<typename T>
		inline T Unbox()
		{
			return *(T*)IMPORT::il2cpp_object_unbox(this);
		}

		template<typename T>
		static Object* BoxValue(const Class* klass, T value)
		{
			if (klass == nullptr)
			{
				throw Exception::NullArgument(
					IL2CPP_NAMEOF(klass)
				);
			}

			if constexpr (!std::is_pointer<T>::value)
			{
				return (Object*)IMPORT::il2cpp_value_box(
					(IMPORT::Il2CppClass*)klass,
					&value
				);
			}
			else 
			{
				return (Object*)IMPORT::il2cpp_value_box(
					(IMPORT::Il2CppClass*)klass,
					value
				);
			}
		}
	};
}