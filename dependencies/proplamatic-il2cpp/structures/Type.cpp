#include "Type.hpp"

namespace IL2CPP
{
	Class* Type::GetClass() const
	{
		return (Class*)IMPORT::il2cpp_class_from_type(this);
	}

	bool Type::IsPassByReference() const
	{
		return this->byref;
	}

	uint32_t Type::GetAttributes() const
	{
		return this->attrs;
	}

	uint32_t Type::GetTypingType() const
	{
		return this->type;
	}
}