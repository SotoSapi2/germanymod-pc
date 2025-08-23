#include "Dumper.hpp"
#include <iostream>
#include <sstream>
#include <vector>
#include <streambuf>
#include <fstream>

namespace IL2CPP
{
	namespace Dumper
	{
		bool is_obfuscated(std::string& str)
		{
			for (unsigned char ch : str)
			{
				if (static_cast<unsigned>(ch) > 127)
				{
					return true;
				}
			}
			return false;
		}

		std::string GenerateMethodPattern(IL2CPP::MethodInfo* method)
		{
			std::stringstream outPut;

			std::string methodTypeName = method->GetReturnClass()->GetName();
			std::string methodName = method->GetName();

			auto modifier = method->GetFlagsAsString();

			outPut << "{" << "\"" << modifier << "\", ";

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

			auto param_count = method->GetParametersCount();
			bool first = true;
			for (size_t i = 0; i < param_count; ++i)
			{
				auto param = method->GetParameterType(i)->GetClass();
				std::string paramTypeName = param->GetName();

				if (param->IsEnum())
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

		std::string GenerateFieldPattern(Class* klass)
		{
			std::stringstream outPut;
			outPut << "\t// FieldPattern: {";

			for (uint16_t i = 0; i < klass->FieldCount(); i++)
			{
				auto fieldClass = klass->GetField(i)->GetType()->GetClass();
				std::string name = fieldClass->GetName();

				if (!is_obfuscated(name))
				{
					outPut << "\"" << name << "\"" << (i < klass->FieldCount() - 1 ? ", " : "");
				}
				else
				{
					outPut << "nullptr" << (i < klass->FieldCount() - 1 ? ", " : "");
				}
			}

			outPut << "};" << std::endl << std::endl;
			return outPut.str(); 
		}

		std::string GenerateField(Class* klass)
		{
			std::stringstream outPut;
			bool isEnum = klass->IsEnum();

			for (size_t index = 0; index < klass->FieldCount(); index++)
			{
				FieldInfo* field = klass->GetField(index);

				outPut << "\t";
				int attrs = field->GetFlags();
				int access = attrs & FIELD_ATTRIBUTE_FIELD_ACCESS_MASK;

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

				Class* fieldClass = field->GetType()->GetClass();
				outPut << fieldClass->GetName() << " " << field->GetName();

				if (field->IsLiteral() && isEnum)
				{
					uint64_t val = field->GetValue<uint64_t>(nullptr);
					outPut << " = " << std::dec << val;
				}

				outPut << std::hex << "; // Index: 0x" << index << " Offset: 0x" << field->GetOffset() << "\n";
			}

			return outPut.str();
		}

		std::string GenerateMethods(Class* klass, bool generatePattern)
		{
			std::stringstream outPut;
			outPut << "\n\t// Methods\n";

			for (size_t index = 0; index < klass->MethodCount(); index++)
			{
				MethodInfo* method = klass->GetMethod(index);

				if (method->GetPointer())
				{
					if (generatePattern)
					{
						outPut << "\t// SignaturePattern: " << GenerateMethodPattern(method) << "\n";
					}

					outPut << "\t// Index: 0x";
					outPut << std::hex << index;
					outPut << " Offset: 0x" << std::hex << (uint64_t)method->GetPointer() - (uint64_t)GetModuleHandleA("GameAssembly.dll");
					outPut << " VA: 0x";
					outPut << std::hex << (uint64_t)method->GetPointer();
				}
				else
				{
					outPut << "\t// Offset: 0x VA: 0x0";
				}
				outPut << "\n\t";

				bool isGeneric = method->IsGeneric();

				outPut << method->GetFlagsAsString() << " ";
				Type* returnType = method->GetReturnType();
				if (returnType->IsPassByReference())
				{
					outPut << "ref ";
				}

				Class* returnClass = returnType->GetClass();
				outPut << returnClass->GetName() << " " << method->GetName() << "(";
				size_t paramCount = method->GetParametersCount();

				for (uint32_t index = 0; index < paramCount; index++)
				{
					Type* param = method->GetParameterType(index);
					auto attrs = param->GetAttributes();
					if (param->IsPassByReference())
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

					outPut << param->GetClass()->GetName() << " " << method->GetParameterName(index) << ", ";
				}
				if (paramCount > 0)
				{
					outPut.seekp(-2, outPut.cur);
				}
				outPut << ") { }" << std::endl << std::endl;
			}
			return outPut.str();
		}

		std::string GenerateType(IL2CPP::Type* type, bool generatePattern)
		{
			std::stringstream outPut;
			Class* klass = type->GetClass();
			outPut << "// Namespace: " << (klass->GetNamespace().empty() ? "none" : klass->GetNamespace());
			outPut << " FieldCount: " << klass->FieldCount();
			outPut << " MethodCount: " << klass->MethodCount() << std::endl;

			uint32_t flags = klass->GetFlags();
			if (flags & TYPE_ATTRIBUTE_SERIALIZABLE)
			{
				outPut << "[Serializable]" << std::endl;
			}

			//TODO attribute
			bool isValueType = klass->IsValueType();
			bool isEnum = klass->IsEnum();
			uint32_t visibility = flags & TYPE_ATTRIBUTE_VISIBILITY_MASK;

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
			else if (!isValueType && !isEnum && flags & TYPE_ATTRIBUTE_SEALED)
			{
				outPut << "sealed ";
			}
			if (flags & TYPE_ATTRIBUTE_INTERFACE)
			{
				outPut << "interface ";
			}
			else if (isEnum)
			{
				outPut << "enum ";
			}
			else if (isValueType)
			{
				outPut << "struct ";
			}
			else
			{
				outPut << "class ";
			}
			outPut << klass->GetName(); //TODO genericContainerIndex
			std::vector<std::string> extends;
			Class* parent = klass->GetParentClass();
			if (!isValueType && !isEnum && parent)
			{
				Type* parent_type = parent->GetType();
				if (parent_type->GetTypingType() != IL2CPP::IMPORT::Il2CppTypeEnum::IL2CPP_TYPE_OBJECT)
				{
					extends.emplace_back(parent->GetName());
				}
			}
			
			for(size_t i = 0; i < klass->InterfaceCount(); i++)
			{
				extends.emplace_back(klass->GetInterface(i)->GetName());
			}

			if (!extends.empty())
			{
				outPut << " : " << extends[0];
				for (int i = 1; i < extends.size(); ++i)
				{
					outPut << ", " << extends[i];
				}
			}
			outPut << std::endl << "{" << std::endl;

			if (generatePattern)
			{
				outPut << GenerateFieldPattern(klass);
			}
			outPut << GenerateField(klass);
			outPut << GenerateMethods(klass, generatePattern);
			//TODO EventInfo
			outPut << "}" << std::endl << std::endl;
			return outPut.str();
		}

		std::string DumpImage(Image* imageToDump, bool generatePattern)
		{
			if (imageToDump == nullptr)
			{
				throw Exception::NullArgument(IL2CPP_NAMEOF(imageToDump));
			}

			std::stringstream imageOutput;
			imageOutput << "// image: " << imageToDump->GetName() << std::endl;

			for (size_t i = 0; i < imageToDump->ClassCount(); i++)
			{
				Class* klass = imageToDump->GetClass(i);
				Type* type = klass->GetType();

				imageOutput << GenerateType(type, generatePattern);
			}

			return imageOutput.str();
		}

		void StartDumper(const std::string& dumpDirectoryPath, bool generatePattern)
		{
			DWORD pathAttribute = GetFileAttributesA(dumpDirectoryPath.c_str());
			if (pathAttribute == INVALID_FILE_ATTRIBUTES || (pathAttribute & FILE_ATTRIBUTE_DIRECTORY) == 0)
			{
				CreateDirectoryA(dumpDirectoryPath.c_str(), NULL);
			}

			for (Image* image : IL2CPP::CurrentDomain()->OpenEveryAssembly())
			{
				auto outPath = std::string(dumpDirectoryPath).append("\\").append(image->GetName()).append(".cs");
				FILE* file = std::fopen(outPath.c_str(), "w");

				if (file == nullptr)
				{
					// TODO: Create exception for file error.
					throw Exception::UnexpectedError();
				}

				std::printf(
					"[INFO] Dumping '%s'.\n", 
					image->GetName().c_str()
				);

				std::string out = DumpImage(image, generatePattern);

				size_t write = std::fwrite(out.c_str(), out.size(), 1, file);
				std::fclose(file);

				std::printf(
					"[INFO] Dumping '%s' image done! Dump file path: %s\n", 
					image->GetName().c_str(), 
					outPath.c_str()
				);
			}
		}
	}
}