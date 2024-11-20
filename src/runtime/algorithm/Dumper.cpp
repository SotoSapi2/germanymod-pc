#include <Windows.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <streambuf>
#include <fstream>
#include <Logger.hpp>
#include <FileUtils.h>
#include "../unity/Unity.hpp"

namespace Dumper
{
	bool is_basic_latin(int32_t x) noexcept
	{
		return x >= 0x0020 && x <= 0x007F;
	}

	bool is_obfuscated(std::string& str)
	{
		std::string out;
		for (auto it = std::find_if(str.begin(), str.end(), is_basic_latin); it != str.end(); it++)
		{
			out += std::string(it, it + 1);
		}

		return out.empty();
	}

	std::string ReadMethodModifier(uint32_t flags)
	{
		std::stringstream outPut;
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

	bool TypeIsByref(const Il2CppType* type)
	{
		auto byref = type->byref;
		if (il2cpp_type_is_byref)
		{
			byref = il2cpp_type_is_byref(type);
		}
		return byref;
	}

	std::string GenerateMethodPattern(const MethodInfo* method)
	{
		std::stringstream outPut;

		std::string methodTypeName = il2cpp_class_get_name(il2cpp_class_from_type(il2cpp_method_get_return_type(method)));
		std::string methodName = il2cpp_method_get_name(method);

		uint32_t iflags = 0;
		auto flags = il2cpp_method_get_flags(method, &iflags);

		auto modif = ReadMethodModifier(flags);
		modif.pop_back();
		outPut << "{" << "\"" << modif << "\", ";

		if (is_obfuscated(methodTypeName))
		{
			outPut << "nullptr" << ", ";
		}
		else
		{
			outPut << "\"" << methodTypeName << "\", ";
		}

		if (is_obfuscated(methodName))
		{
			outPut << "nullptr" << ", {";
		}
		else
		{
			outPut << "\"" << methodName << "\", {";
		}

		auto param_count = il2cpp_method_get_param_count(method);
		bool first = true;
		for (int i = 0; i < param_count; ++i)
		{
			auto param = il2cpp_method_get_param(method, i);
			std::string paramTypeName = il2cpp_class_from_type(param)->name;

			if (il2cpp_class_is_enum(il2cpp_class_from_type(param)))
			{
				outPut << (first ? "\"ENUM\"" : ", \"ENUM\"");
				first = false;
			}
			else if (is_obfuscated(paramTypeName))
			{
				outPut << (first ? "nullptr" : ", nullptr");
				first = false;
			}
			else
			{
				outPut << (first ? "\"" : ", \"") << paramTypeName << "\"";
				first = false;
			}
		}

		outPut << "}};";

		return outPut.str();
	}

	std::string GenerateProperty(Il2CppClass* klass)
	{
		std::stringstream outPut;
		outPut << "\n\t// Properties\n";
		void* iter = nullptr;
		while (auto prop_const = il2cpp_class_get_properties(klass, &iter))
		{
			//TODO attribute
			auto prop = const_cast<PropertyInfo*>(prop_const);
			auto get = il2cpp_property_get_get_method(prop);
			auto set = il2cpp_property_get_set_method(prop);
			auto prop_name = il2cpp_property_get_name(prop);
			outPut << "\t";
			Il2CppClass* prop_class = nullptr;
			uint32_t iflags = 0;
			if (get)
			{
				outPut << ReadMethodModifier(il2cpp_method_get_flags(get, &iflags));
				prop_class = il2cpp_class_from_type(il2cpp_method_get_return_type(get));
			}
			else if (set)
			{
				outPut << ReadMethodModifier(il2cpp_method_get_flags(set, &iflags));
				auto param = il2cpp_method_get_param(set, 0);
				prop_class = il2cpp_class_from_type(param);
			}
			if (prop_class)
			{
				outPut << il2cpp_class_get_name(prop_class) << " " << prop_name << " { ";
				if (get)
				{
					outPut << "get; ";
				}
				if (set)
				{
					outPut << "set; ";
				}
				outPut << "}\n";
			}
			else
			{
				if (prop_name)
				{
					outPut << " // unknown property " << prop_name;
				}
			}
		}
		return outPut.str();
	}

	std::string GenerateFieldPattern(Il2CppClass* klass)
	{
		std::stringstream outPut;
		outPut << "\t// FieldPattern: {";

		void* iter = nullptr;
		while (auto field = il2cpp_class_get_fields(klass, &iter))
		{

			auto fieldClass = il2cpp_class_from_type(il2cpp_field_get_type(field));
			std::string name = il2cpp_class_get_name(fieldClass);

			if (!is_obfuscated(name))
			{
				outPut << "\"" << il2cpp_class_get_name(fieldClass) << "\", ";
			}
			else
			{
				outPut << "nullptr, ";
			}
		}

		auto strOut = outPut.str();

		if (strOut.length() > 18)
		{
			strOut.pop_back();
			strOut.pop_back();
		}

		return strOut + "};\n\n";
	}

	std::string GenerateField(Il2CppClass* klass)
	{
		std::stringstream outPut;
		auto is_enum = il2cpp_class_is_enum(klass);
		void* iter = nullptr;

		int index = 0;

		while (auto field = il2cpp_class_get_fields(klass, &iter))
		{
			//TODO attribute
			outPut << "\t";
			auto attrs = il2cpp_field_get_flags(field);
			auto access = attrs & FIELD_ATTRIBUTE_FIELD_ACCESS_MASK;
			switch (access)
			{
				case FIELD_ATTRIBUTE_PRIVATE:
					outPut << "private ";
					break;
				case FIELD_ATTRIBUTE_PUBLIC:
					outPut << "public ";
					break;
				case FIELD_ATTRIBUTE_FAMILY:
					outPut << "protected ";
					break;
				case FIELD_ATTRIBUTE_ASSEMBLY:
				case FIELD_ATTRIBUTE_FAM_AND_ASSEM:
					outPut << "internal ";
					break;
				case FIELD_ATTRIBUTE_FAM_OR_ASSEM:
					outPut << "protected internal ";
					break;
			}
			if (attrs & FIELD_ATTRIBUTE_LITERAL)
			{
				outPut << "const ";
			}
			else
			{
				if (attrs & FIELD_ATTRIBUTE_STATIC)
				{
					outPut << "static ";
				}
				if (attrs & FIELD_ATTRIBUTE_INIT_ONLY)
				{
					outPut << "readonly ";
				}
			}
			auto field_type = il2cpp_field_get_type(field);
			auto field_class = il2cpp_class_from_type(field_type);
			outPut << il2cpp_class_get_name(field_class) << " " << il2cpp_field_get_name(field);
			//TODO 获取构造函数初始化后的字段值
			if (attrs & FIELD_ATTRIBUTE_LITERAL && is_enum)
			{
				uint64_t val = 0;
				il2cpp_field_static_get_value(field, &val);
				outPut << " = " << std::dec << val;
			}
			outPut << std::hex << "; // Index: 0x" << index << " Offset: 0x" << il2cpp_field_get_offset(field) << "\n";

			index++;

		}
		return outPut.str();
	}

	std::string GenerateMethods(Il2CppClass* klass, bool generatePattern)
	{
		std::stringstream outPut;
		outPut << "\n\t// Methods\n";

		int index = 0;

		void* iter = nullptr;

		while (auto method = il2cpp_class_get_methods(klass, &iter))
		{
			//TODO attribute

			if (method->methodPointer)
			{
				if (generatePattern)
				{
					outPut << "\t// SignaturePattern: " << GenerateMethodPattern(method) << "\n";
				}

				outPut << "\t// Index: 0x";
				outPut << std::hex << index;
				outPut << " Offset: 0x" << std::hex << (uint64_t)method->methodPointer - (uint64_t)GetModuleHandleA("GameAssembly.dll");
				outPut << " VA: 0x";
				outPut << std::hex << (uint64_t)method->methodPointer;
			}
			else
			{
				outPut << "\t// Offset: 0x VA: 0x0";
			}
			/*if (method->slot != 65535) {
				outPut << " Slot: " << std::dec << method->slot;
			}*/
			outPut << "\n\t";
			uint32_t iflags = 0;
			bool isGeneric = method->is_generic;
			auto flags = il2cpp_method_get_flags(method, &iflags);
			outPut << ReadMethodModifier(flags);
			//TODO genericContainerIndex
			auto return_type = il2cpp_method_get_return_type(method);
			if (TypeIsByref(return_type))
			{
				outPut << "ref ";
			}
			auto return_class = il2cpp_class_from_type(return_type);
			outPut << il2cpp_class_get_name(return_class) << " " << il2cpp_method_get_name(method)
				<< "(";
			auto param_count = il2cpp_method_get_param_count(method);

			for (uint32_t i = 0; i < param_count; ++i)
			{
				auto param = il2cpp_method_get_param(method, i);
				auto attrs = param->attrs;
				if (TypeIsByref(param))
				{
					if (attrs & PARAM_ATTRIBUTE_OUT && !(attrs & PARAM_ATTRIBUTE_IN))
					{
						outPut << "out ";
					}
					else if (attrs & PARAM_ATTRIBUTE_IN && !(attrs & PARAM_ATTRIBUTE_OUT))
					{
						outPut << "in ";
					}
					else
					{
						outPut << "ref ";
					}
				}
				else
				{
					if (attrs & PARAM_ATTRIBUTE_IN)
					{
						outPut << "[In] ";
					}
					if (attrs & PARAM_ATTRIBUTE_OUT)
					{
						outPut << "[Out] ";
					}
				}
				auto parameter_class = il2cpp_class_from_type(param);

				/*if(method->is_generic)
				{
					outPut << method->name;

					if(method->is_inflated && method->is_generic && method->genericMethod->context.method_inst)
					{
						const Il2CppGenericInst* inst = method->genericMethod->context.method_inst;

						outPut << il2cpp_type_get_name(inst->type_argv[i]);
						if(i < inst->type_argc - 1)
							outPut << ", ";
					}
				}
				else
				{*/
				outPut << il2cpp_class_get_name(parameter_class)
					<< " "
					<< il2cpp_method_get_param_name(method, i);
				outPut << ", ";
				//}
			}
			if (param_count > 0)
			{
				outPut.seekp(-2, outPut.cur);
			}
			outPut << ") { }\n\n";

			index++;
			//TODO GenericInstMethod
		}
		return outPut.str();
	}

	std::string GenerateType(const Il2CppType* type, bool generatePattern)
	{

		std::stringstream outPut;
		auto* klass = il2cpp_class_from_type(type);
		LOG_INFO("Dumping %s", il2cpp_class_get_name(klass));
		outPut << "\n// Namespace: " << (strlen(il2cpp_class_get_namespace(klass)) == 0 ? "none" : il2cpp_class_get_namespace(klass))
			<< " ClassDataSize: " << il2cpp_class_get_data_size(il2cpp_class_from_type(type))
			<< " FieldCount: " << klass->field_count
			<< " MethodCount: " << klass->method_count << "\n";

		auto flags = il2cpp_class_get_flags(klass);
		if (flags & TYPE_ATTRIBUTE_SERIALIZABLE)
		{
			outPut << "[Serializable]\n";
		}
		//TODO attribute
		auto is_valuetype = il2cpp_class_is_valuetype(klass);
		auto is_enum = il2cpp_class_is_enum(klass);
		auto visibility = flags & TYPE_ATTRIBUTE_VISIBILITY_MASK;
		switch (visibility)
		{
			case TYPE_ATTRIBUTE_PUBLIC:
			case TYPE_ATTRIBUTE_NESTED_PUBLIC:
				outPut << "public ";
				break;
			case TYPE_ATTRIBUTE_NOT_PUBLIC:
			case TYPE_ATTRIBUTE_NESTED_FAM_AND_ASSEM:
			case TYPE_ATTRIBUTE_NESTED_ASSEMBLY:
				outPut << "internal ";
				break;
			case TYPE_ATTRIBUTE_NESTED_PRIVATE:
				outPut << "private ";
				break;
			case TYPE_ATTRIBUTE_NESTED_FAMILY:
				outPut << "protected ";
				break;
			case TYPE_ATTRIBUTE_NESTED_FAM_OR_ASSEM:
				outPut << "protected internal ";
				break;
		}
		if (flags & TYPE_ATTRIBUTE_ABSTRACT && flags & TYPE_ATTRIBUTE_SEALED)
		{
			outPut << "static ";
		}
		else if (!(flags & TYPE_ATTRIBUTE_INTERFACE) && flags & TYPE_ATTRIBUTE_ABSTRACT)
		{
			outPut << "abstract ";
		}
		else if (!is_valuetype && !is_enum && flags & TYPE_ATTRIBUTE_SEALED)
		{
			outPut << "sealed ";
		}
		if (flags & TYPE_ATTRIBUTE_INTERFACE)
		{
			outPut << "interface ";
		}
		else if (is_enum)
		{
			outPut << "enum ";
		}
		else if (is_valuetype)
		{
			outPut << "struct ";
		}
		else
		{
			outPut << "class ";
		}
		outPut << il2cpp_class_get_name(klass); //TODO genericContainerIndex
		std::vector<std::string> extends;
		auto parent = il2cpp_class_get_parent(klass);
		if (!is_valuetype && !is_enum && parent)
		{
			auto parent_type = il2cpp_class_get_type(parent);
			if (parent_type->type != IL2CPP_TYPE_OBJECT)
			{
				extends.emplace_back(il2cpp_class_get_name(parent));
			}
		}
		void* iter = nullptr;
		while (auto itf = il2cpp_class_get_interfaces(klass, &iter))
		{
			extends.emplace_back(il2cpp_class_get_name(itf));
		}
		if (!extends.empty())
		{
			outPut << " : " << extends[0];
			for (int i = 1; i < extends.size(); ++i)
			{
				outPut << ", " << extends[i];
			}
		}
		outPut << "\n{\n\n";

		if (generatePattern)
		{
			outPut << GenerateFieldPattern(klass);
		}
		outPut << GenerateField(klass);
		outPut << GenerateProperty(klass);
		outPut << GenerateMethods(klass, generatePattern);
		//TODO EventInfo
		outPut << "}\n";
		return outPut.str();
	}

	void DumpImage(const char* assemblyName, const char* outDir, bool generatePattern)
	{
		LOG_INFO("dumping... %s", assemblyName);
		LOG_INFO("outdir %s\\%s.cs", outDir, assemblyName);

		auto domain = il2cpp_domain_get();

		if (domain == nullptr)
		{
			LOG_INFO("cannot get domain.");
			return;
		}

		auto assembly = il2cpp_domain_assembly_open(domain, assemblyName);

		if (assembly == nullptr)
		{
			LOG_INFO("no assembly named %s.");
			return;
		}
		auto image = il2cpp_assembly_get_image(assembly);

		std::stringstream imageOutput;
		imageOutput << "// Image " << il2cpp_image_get_name(image) << "\n";

		std::vector<std::string> outPuts;
		std::stringstream imageStr;
		imageStr << "\n// Dll : " << il2cpp_image_get_name(image);
		auto classCount = il2cpp_image_get_class_count(image);
		for (int j = 0; j < classCount; ++j)
		{
			auto klass = il2cpp_image_get_class(image, j);
			auto type = il2cpp_class_get_type(const_cast<Il2CppClass*>(klass));
			//LOGD("type name : %s", il2cpp_type_get_name(type));
			auto outPut = imageStr.str() + GenerateType(type, generatePattern);
			outPuts.push_back(outPut);
		}

		LOG_INFO("write dump file");

		auto outPath = std::string(outDir).append("\\").append(assemblyName).append(".cs");

		std::string out;
		out.append(imageOutput.str());

		for (int i = 0; i < outPuts.size(); ++i)
		{
			out.append(outPuts[i]);
		}

		writeFile(outPath, out);
		
		LOG_INFO("dump done! %s", outPath.c_str());
	}
}