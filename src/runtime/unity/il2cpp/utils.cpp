#include "utils.hpp"
#include <Windows.h>
#include <Logger.hpp>


namespace Il2CppUtils
{
	template<typename T>
	T UnboxIl2CppObject(Il2CppObject* obj)
	{
		return *static_cast<T*>(il2cpp_object_unbox(obj));
	}

	FieldPtr GetFieldPointer(Il2CppClass* klass, void* instancePtr, const char* name, int indexOffset)
	{
		if (klass == nullptr)
		{
			LOG_ERROR("First arg (Il2CppClass) is null. %s", name);
			return 0;
		}

		if (instancePtr == nullptr)
		{
			LOG_ERROR("Second arg (void*) Argument is null. %s", name);
			return 0;
		}

		if (!klass->fields)
		{
			LOG_ERROR("Fields uninitialized.");
			return 0;
		}

		for (int i = 0; i < klass->field_count; i++)
		{
			auto field = &klass->fields[i];

			if (strcmp(name, field->name) == 0)
			{
				int offset = klass->fields[i + indexOffset].offset;
				return (FieldPtr)instancePtr + offset;
			}
		}

		LOG_ERROR("Can't find wanted Field. %s", name);
		return 0;
	}

	FieldPtr GetFieldPointerByIndex(Il2CppClass* klass, Il2CppObject* instancePtr, int index)
	{
		if (klass == nullptr)
		{
			LOG_ERROR("[PROPLAM] First arg (Il2CppClass) is null");
			return 0;
		}

		if (instancePtr == nullptr)
		{
			LOG_ERROR("Second arg (void*) Argument is null");
			return 0;
		}

		if (index > klass->field_count)
		{
			LOG_ERROR("Index is out of bound");
			return 0;
		}

		if (!klass->fields)
		{
			LOG_ERROR("Fields uninitialized.");
			return 0;
		}
		else
		{
			int offset = klass->fields[index].offset;
			return (FieldPtr)instancePtr + offset;
		}

		return 0;
	}

	void SetStaticFieldByIndex(Il2CppClass* klass, int index, void* value)
	{
		if (klass == nullptr)
		{
			return;
		}

		if (klass->field_count > 0 && klass->fields)
		{
			il2cpp_field_static_set_value(&klass->fields[index], value);
		}
	}

	bool CheckFieldPattern(Il2CppClass* klass, size_t fieldCount, size_t methodCount, const FieldPattern& pattern)
	{
		if (klass == nullptr)
		{
			LOG_INFO("First arg (Il2CppClass*) is null.");
			return false;
		}

		if (klass->field_count != fieldCount && klass->field_count != pattern.size() && klass->method_count != methodCount)
		{
			return false;
		}

		size_t correctFieldCount = 0;
		size_t scanIndex = 0;
		void* iter = nullptr;
		while (auto field = il2cpp_class_get_fields(klass, &iter))
		{
			auto fieldClass = il2cpp_class_from_type(il2cpp_field_get_type(field));

			if (fieldClass == nullptr)
			{
				continue;
			}

			if (scanIndex >= pattern.size())
			{
				break;
			}

			if (pattern[scanIndex] == nullptr)
			{
				correctFieldCount++;
			}
			else if (strcmp(il2cpp_class_get_name(fieldClass), pattern[scanIndex]) == 0)
			{
				correctFieldCount++;
			}

			scanIndex++;
		}

		return correctFieldCount == pattern.size();
	}

	std::string ModifierFlagsToString(uint32_t flags)
	{
		std::stringstream outPut{};
		auto access = flags & METHOD_ATTRIBUTE_MEMBER_ACCESS_MASK;

		switch (access)
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

		return outPut.str();
	}

	std::string SignaturePatternToString(const SignaturePattern& pattern)
	{
		#define HANDLE_NULL_STR(str) (std::string(str == nullptr ? "???" : str) + " ")
		std::string out = HANDLE_NULL_STR(pattern.modifier) + HANDLE_NULL_STR(pattern.typeName) + HANDLE_NULL_STR(pattern.name) + " ";

		if (!pattern.paramTypeName.empty())
		{
			bool first = true;

			std::string str;

			for (int i = 0; i < std::size(pattern.paramTypeName); i++)
			{
				str += first ? HANDLE_NULL_STR(pattern.paramTypeName[i]) : ", " + HANDLE_NULL_STR(pattern.paramTypeName[i]);
				first = false;
			}

			out += "(" + str + ")";
		}
		#undef HANDLE_NULL_STR
		return out;
	}

	void WaitForMethodsinitialization(Il2CppClass* klass)
	{
		int sleepCounter = 0;
		while (klass->methods == nullptr && sleepCounter < 5000)
		{
			Sleep(1);
			sleepCounter++;
			if (sleepCounter == 10)
			{
				// force init coz il2cpp
				//il2cpp_runtime_class_init(klass);
				il2cpp_object_new(klass);
			}

			if (sleepCounter == 1000)
			{
				LOG_WARN("Deadlock occurred while waiting for methods initialization.");
			}
		}
	}

	MethodPtr GetMethodPointer(Il2CppClass* klass, const SignaturePattern& pattern, int indexOffset)
	{
		if (klass == nullptr)
		{
			LOG_ERROR("First arg (Il2CppClass*) is null. Target: %s", SignaturePatternToString(pattern).c_str());
			return 0;
		}

		//LOGI("Finding %s", TraceMethod(pattern).c_str());

		if (klass->method_count <= 0)
		{
			LOG_ERROR("No method found in %s", il2cpp_class_get_name(klass));
			return 0;
		}

		WaitForMethodsinitialization(klass);

		for (int i = 0; i < klass->method_count; i++)
		{
			const MethodInfo* method = klass->methods[i];

			int checkScore = 0;

			// 0 = fail, 1 = skipped, 2 = success.
			int test1 = 0, test2 = 0, test3 = 0, test4 = 0;

			uint32_t iflags = 0;
			uint32_t flags = il2cpp_method_get_flags(method, &iflags);

			std::string methodName = il2cpp_method_get_name(method);
			std::string methodModif = ModifierFlagsToString(flags);
			std::string methodTypeName = il2cpp_class_from_type(il2cpp_method_get_return_type(method))->name;
			int methodParamCount = il2cpp_method_get_param_count(method);

			methodModif.pop_back();

			//LOGI("Name Check %s", BOOL_TO_STRING(pattern.name == methodName));
			if (pattern.name == nullptr)
			{
				test1 = 1;
				checkScore++;
			}
			else if (methodName == pattern.name)
			{

				test1 = 2;
				checkScore++;
			}
			else
			{
				continue;
			}

			//LOGI("modifier Check %s", BOOL_TO_STRING(methodModif == pattern.modifier));
			if (pattern.modifier == nullptr)
			{
				test2 = 1;
				checkScore++;
			}
			else if (methodModif == pattern.modifier)
			{
				test2 = 2;
				checkScore++;
			}
			else
			{
				continue;
			}

			//LOGI("modifier Check %s", BOOL_TO_STRING(pattern.typeName == methodTypeName));
			if (pattern.typeName == nullptr)
			{
				test3 = 1;
				checkScore++;
			}
			else if (methodTypeName == pattern.typeName)
			{
				test3 = 2;
				checkScore++;
			}
			else
			{
				continue;
			}

			//LOGI("paramCount Check %s", BOOL_TO_STRING(pattern.paramCount == methodParamCount));
			if (pattern.paramTypeName.size() > 0 && pattern.paramTypeName.size() == methodParamCount)
			{
				int successParamScore = 0;

				//LOGI("param Check");
				for (int i2 = 0; i2 < pattern.paramTypeName.size(); ++i2)
				{
					if (pattern.paramTypeName[i2] != nullptr)
					{
						std::string typeName = pattern.paramTypeName[i2];
						const Il2CppType* paramType = il2cpp_method_get_param(method, i2);
						Il2CppClass* paramClass = il2cpp_class_from_type(paramType);
						bool is_enum = il2cpp_class_is_enum(paramClass);
						bool is_value = il2cpp_class_is_valuetype(paramClass);

						if (strcmp(typeName.c_str(), "ENUM") == 0 && is_enum)
						{
							successParamScore++;
						}
						else if (strcmp(typeName.c_str(), "VALUE") == 0 && is_value && !is_enum)
						{
							successParamScore++;
						}
						else if (strcmp(typeName.c_str(), paramClass->name) == 0)
						{
							successParamScore++;
						}
					}
					else if (pattern.paramTypeName[i2] == nullptr)
					{
						successParamScore++;
					}
				}

				if (successParamScore == pattern.paramTypeName.size())
				{
					test4 = 2;
					checkScore++;
				}
				else
				{
					continue;
				}
			}
			else if (pattern.paramTypeName.size() == 0)
			{
				test4 = 1;
				checkScore++;
			}

			if (checkScore >= 4)
			{
				return (MethodPtr)klass->methods[i + indexOffset]->methodPointer;
			}
		}

		LOG_ERROR("Can't find wanted method in %s. Target: %s", klass->name, SignaturePatternToString(pattern).c_str());
		return 0;
	}

	MethodPtr GetMethodPointerByName(Il2CppClass* klass, const char* name, int sameMethodNameSkip, int indexOffset)
	{
		if (klass == nullptr)
		{
			LOG_ERROR("First arg (Il2CppClass) is null. %s", name);
			return 0;
		}

		//const MethodInfo* method = il2cpp_class_get_method_from_name(klass, name, argsCount); // this thing is broken.

		if (klass->method_count <= 0)
		{
			LOG_ERROR("No method found in %s", il2cpp_class_get_name(klass));
			return 0;
		}

		WaitForMethodsinitialization(klass);

		int nig = 0;
		for (int i = 0; i < klass->method_count; i++)
		{
			const MethodInfo* method = klass->methods[i];
			if (strcmp(il2cpp_method_get_name(method), name) == 0)
			{

				if (nig < sameMethodNameSkip)
				{
					nig++;
					continue;
				}

				if (i + indexOffset > klass->method_count)
				{
					LOG_ERROR("Index is out of bound. pointed index %i.", i + indexOffset);
					return nullptr;
				}

				return (MethodPtr)klass->methods[i + indexOffset]->methodPointer;
			}
		}

		LOG_ERROR("Can't find %s", name);
		return 0;
	}

	MethodPtr GetMethodPointerByIndex(Il2CppClass* klass, int index)
	{

		if (klass == nullptr)
		{
			LOG_ERROR("First arg (Il2CppClass) is null");
			return nullptr;
		}

		if (klass->method_count <= 0)
		{
			LOG_ERROR("No method found in %s", il2cpp_class_get_name(klass));
			return nullptr;
		}

		if (index > klass->method_count)
		{
			LOG_ERROR("Index is out of bound. (method_count %i)", klass->method_count);
			return nullptr;
		}

		WaitForMethodsinitialization(klass);

		if (klass->methods == nullptr)
		{
			int i = 0;
			void* iter = nullptr;
			while (auto method = il2cpp_class_get_methods(klass, &iter))
			{
				if (i == index)
				{
					return (MethodPtr)method->methodPointer;
				}
				index++;
			}
		}
		else
		{
			const MethodInfo* method = klass->methods[index];

			if (method != nullptr)
			{
				return (MethodPtr)method->methodPointer;
			}
		}


		LOG_ERROR("Fail to find method from index %i", index);
		return nullptr;
	}
}