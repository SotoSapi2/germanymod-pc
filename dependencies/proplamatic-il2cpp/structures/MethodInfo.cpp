#include "MethodInfo.hpp"
#include <sstream>

namespace IL2CPP
{
	uint32_t MethodInfo::GetFlags() const
	{
		return this->flags;
	}

	std::string MethodInfo::GetFlagsAsString() const
	{
		std::stringstream outPut{};

		switch (GetFlags() & METHOD_ATTRIBUTE_MEMBER_ACCESS_MASK)
		{
		case METHOD_ATTRIBUTE_PRIVATE:
			outPut << "private ";
			break;
		case METHOD_ATTRIBUTE_PUBLIC:
			outPut << "public ";
			break;
		case METHOD_ATTRIBUTE_FAMILY:
			outPut << "protected ";
			break;
		case METHOD_ATTRIBUTE_ASSEM:
		case METHOD_ATTRIBUTE_FAM_AND_ASSEM:
			outPut << "internal ";
			break;
		case METHOD_ATTRIBUTE_FAM_OR_ASSEM:
			outPut << "protected internal ";
			break;
		default:
			outPut << "unknown";
		}

		if (flags & METHOD_ATTRIBUTE_STATIC)
		{
			outPut << "static ";
		}
		if (flags & METHOD_ATTRIBUTE_ABSTRACT)
		{
			outPut << "abstract ";
			if ((flags & METHOD_ATTRIBUTE_VTABLE_LAYOUT_MASK) == METHOD_ATTRIBUTE_REUSE_SLOT)
			{
				outPut << "override ";
			}
		}
		else if (flags & METHOD_ATTRIBUTE_FINAL)
		{
			if ((flags & METHOD_ATTRIBUTE_VTABLE_LAYOUT_MASK) == METHOD_ATTRIBUTE_REUSE_SLOT)
			{
				outPut << "sealed override ";
			}
		}
		else if (flags & METHOD_ATTRIBUTE_VIRTUAL)
		{
			if ((flags & METHOD_ATTRIBUTE_VTABLE_LAYOUT_MASK) == METHOD_ATTRIBUTE_NEW_SLOT)
			{
				outPut << "virtual ";
			}
			else
			{
				outPut << "override ";
			}
		}
		if (flags & METHOD_ATTRIBUTE_PINVOKE_IMPL)
		{
			outPut << "extern ";
		}

		std::string out = outPut.str();
		out.pop_back();

		return out;
	}

	uint16_t MethodInfo::GetIflags() const
	{
		return this->iflags;
	}

	Type* MethodInfo::GetReturnType() const
	{
		return (Type*)this->return_type;
	}

	size_t MethodInfo::GetParametersCount() const
	{
		return this->parameters_count;
	}

	Type* MethodInfo::GetParameterType(size_t index) const
	{
		if (index >= GetParametersCount())
		{
			throw Exception::IndexOutOfBounds(index, GetParametersCount());
		}

		return (Type*)this->parameters[index];
	}

	std::string MethodInfo::GetParameterName(uint32_t index) const
	{
		return IMPORT::il2cpp_method_get_param_name(this, index);
	}

	Class* MethodInfo::GetReturnClass() const
	{
		return GetReturnType()->GetClass();
	}

	std::string MethodInfo::GetName() const
	{
		return this->name;
	}

	bool MethodInfo::IsGeneric() const
	{
		return this->is_generic;
	}

	void* MethodInfo::GetPointer() const
	{
		return this->methodPointer;
	}
}
