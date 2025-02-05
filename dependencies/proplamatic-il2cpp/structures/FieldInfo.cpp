#include "FieldInfo.hpp"

namespace IL2CPP
{
	int32_t FieldInfo::GetOffset() const
	{
		return this->offset;
	}

	std::string FieldInfo::GetName() const
	{
		return this->name;
	}

	Type* FieldInfo::GetType() const
	{
		return (Type*)this->type;
	}

	int32_t FieldInfo::IsStatic() const
	{
		return (this->type->attrs & FIELD_ATTRIBUTE_STATIC) != 0;
	}

	int32_t FieldInfo::IsLiteral() const
	{
		return (this->type->attrs & FIELD_ATTRIBUTE_LITERAL) != 0;
	}

	int32_t FieldInfo::GetFlags() const
	{
		return this->type->attrs;
	}
}