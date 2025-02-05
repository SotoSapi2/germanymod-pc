#include "Class.hpp"
#include <sstream>

namespace IL2CPP
{
	Object* Class::Create(Class* klass)
	{
		return (Object*)IMPORT::il2cpp_object_new(klass);
	}

	void Class::ForceInitializeClass() const
	{
		IL2CPP::VM_FUNCTIONS::Class::Init((Class*)this);
	}

	Class* Class::GetParentClass() const
	{
		return (Class*)this->parent;
	}

	size_t Class::InterfaceCount() const
	{
		return this->interfaces_count;
	}

	Class* Class::GetInterface(size_t index) const
	{
		if (this->implementedInterfaces == nullptr)
		{
			ForceInitializeClass();
		}

		return (Class*)this->implementedInterfaces[index];
	}

	std::string Class::GetNamespace() const
	{
		return this->namespaze;
	}

	std::string Class::GetName() const
	{
		return this->name;
	}

	std::string Class::GetFullName() const
	{
		auto _namespace = GetNamespace();
		return _namespace.empty() ? GetName() : _namespace.append(".").append(GetName());
	}

	uint16_t Class::FieldCount() const
	{
		return this->field_count;
	}

	FieldInfo* Class::GetField(size_t index) const
	{
		if (index >= this->FieldCount())
		{
			throw Exception::IndexOutOfBounds(index, this->FieldCount());
		}

		if (this->HasFields() && this->fields == nullptr)
		{
			ForceInitializeClass();
		}

		return (FieldInfo*)&this->fields[index];
	}

	FieldInfo* Class::GetField(const std::string& fieldName, int32_t indexOffset) const
	{
		if (this->HasFields() && this->fields == nullptr)
		{
			ForceInitializeClass();
		}

		for (size_t i = 0; i < FieldCount(); i++)
		{
			FieldInfo* current = GetField(i);
			if (current->GetName() == fieldName)
			{
				return GetField(i + indexOffset);
			}
		}

		throw Exception::NoSuchMember(
			Exception::ExpectedMemberType::FIELD,
			this->name,
			fieldName
		);

		return nullptr;
	}

	bool Class::HasFields() const
	{
		return this->field_count > 0;
	}

	bool Class::IsFieldsInitialized() const
	{
		return this->fields != nullptr;
	}

	uint16_t Class::MethodCount() const
	{
		return this->method_count;
	}

	MethodInfo* Class::GetMethod(size_t index) const
	{
		if (this->HasMethods() && this->methods == nullptr)
		{
			ForceInitializeClass();
		}

		if (index >= MethodCount())
		{
			throw Exception::IndexOutOfBounds(index, MethodCount());
		}

		return (MethodInfo*)this->methods[index];
	}

	MethodInfo* Class::GetMethod(const std::string& methodName) const
	{
		if (this->HasMethods() && this->methods == nullptr)
		{
			ForceInitializeClass();
		}

		for (size_t i = 0; i < MethodCount(); i++)
		{
			MethodInfo* current = this->GetMethod(i);
			if (current->GetName() == methodName)
			{
				return current;
			}
		}

		throw Exception::NoSuchMember(
			Exception::ExpectedMemberType::METHOD,
			this->name,
			methodName
		);

		return nullptr;
	}

	MethodInfo* Class::GetMethod(const std::string& methodName, size_t paramsCount) const
	{
		if (this->HasMethods() && this->methods == nullptr)
		{
			ForceInitializeClass();
		}

		for (size_t i = 0; i < MethodCount(); i++)
		{
			MethodInfo* current = this->GetMethod(i);
			if (current->GetName() == methodName)
			{
				return current;
			}
		}

		throw Exception::NoSuchMember(
			Exception::ExpectedMemberType::METHOD,
			this->name,
			methodName
		);

		return nullptr;
	}

	MethodInfo* Class::GetMethod(const std::string& methodName, size_t sameMethodNameSkip, int indexOffset) const
	{
		if (this->HasMethods() && this->methods == nullptr)
		{
			ForceInitializeClass();
		}

		uint32_t sameNameCounter = 0;

		for (size_t i = 0; i < MethodCount(); i++)
		{
			MethodInfo* current = this->GetMethod(i);
			if (current->GetName() != methodName) continue;

			if (sameNameCounter == sameMethodNameSkip)
			{
				size_t index = i - indexOffset;

				if (index >= MethodCount() || index < 0)
				{
					throw Exception::IndexOutOfBounds(index, MethodCount());
				}

				return this->GetMethod(index);
			}

			sameMethodNameSkip++;
		}

		throw Exception::NoSuchMember(
			Exception::ExpectedMemberType::METHOD,
			this->name,
			methodName
		);

		return nullptr;
	}

	MethodInfo* Class::GetMethodByPattern(const SignaturePattern& pattern, int indexOffset) const
	{
		if (this->HasMethods() && this->methods == nullptr)
		{
			ForceInitializeClass();
		}

		for (size_t i = 0; i < MethodCount(); i++)
		{
			MethodInfo* method = this->GetMethod(i);

			std::string methodName = method->GetName();
			std::string methodModifier = method->GetFlagsAsString();
			std::string methodTypeName = method->GetReturnType()->GetClass()->GetName();
			size_t methodParamCount = method->GetParametersCount();

			if (pattern.methodName != nullptr && methodName != pattern.methodName)
			{
				continue;
			}

			if (pattern.modifier != nullptr && methodModifier != pattern.modifier)
			{
				continue;
			}

			if (pattern.typeName != nullptr && methodTypeName != pattern.typeName)
			{
				continue;
			}
			if (!pattern.parameterTypeList.empty())
			{
				if (pattern.parameterTypeList.size() != method->GetParametersCount())
				{
					continue;
				}

				size_t successParamCounter = 0;

				for(size_t j = 0; j < method->GetParametersCount(); j++)
				{
					const char* typeName = pattern.parameterTypeList[j];

					if (typeName == nullptr)
					{
						successParamCounter++;
						continue;
					}

					Class* paramClass = method->GetParameterType(j)->GetClass();

					if (paramClass->IsEnum() && strcmp(typeName, "ENUM") == 0)
					{
						successParamCounter++;
					}
					else if (paramClass->GetName() == typeName)
					{
						successParamCounter++;
					}
				}

				if (successParamCounter == method->GetParametersCount())
				{
					return this->GetMethod(i + indexOffset);
				}
			}
			else
			{
				return this->GetMethod(i + indexOffset);
			}
		}

		throw Exception::PatternScanFailed();
		return nullptr;
	}



	MethodInfo* Class::GetMethodByPatternNullable(const SignaturePattern& pattern, int indexOffset) const
	{
		try
		{
			return GetMethodByPattern(pattern, indexOffset);
		}
		catch (const Exception::PatternScanFailed& err)
		{
			return nullptr;
		}
	}

	bool Class::CompareFieldPattern(const FieldPattern* fieldPattern) const
	{
		if (fieldPattern->size() != FieldCount())
		{
			return false;
		}

		size_t correctFieldCounter = 0;

		for (size_t i = 0; i < FieldCount(); i++)
		{
			Class* fieldClass = this->GetField(i)->GetType()->GetClass();

			const char* currentPattern = fieldPattern->at(i);

			if (currentPattern == nullptr)
			{
				correctFieldCounter++;
			}
			else if (fieldClass->GetName() == currentPattern)
			{
				correctFieldCounter++;
			}
		}

		return correctFieldCounter == FieldCount();
	}

	bool Class::HasMethods() const
	{
		return this->method_count > 0;
	}

	bool Class::IsMethodsInitialized() const
	{
		return this->methods != nullptr;
	}

	bool Class::IsValueType() const
	{
		return this->byval_arg.valuetype;
	}

	bool Class::IsEnum() const
	{
		return IMPORT::il2cpp_class_is_enum(this); //this->enumtype;
	}

	uint32_t Class::GetFlags() 
	{
		return this->flags;
	}

	Type* Class::GetType() const
	{
		return (Type*)IMPORT::il2cpp_class_get_type(
			(Class*)this
		);
	}
}