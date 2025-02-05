#include "PatternStructs.hpp"
#include <sstream>

namespace IL2CPP
{
	SignaturePattern::SignaturePattern(const char* modifier, const char* typeName, const char* methodName, std::vector<const char*> parameterTypeList)
	{
		this->modifier = modifier;
		this->typeName = typeName;
		this->methodName = methodName;
		this->parameterTypeList = parameterTypeList;
	}

	std::string SignaturePattern::ToString() const
	{
		std::stringstream out;
		out << "{";

		if (this->modifier != nullptr)
		{
			out << "\"" << this->modifier << "\"" << ", ";
		}
		else
		{
			out << "nullptr" << ", ";
		}

		if (this->typeName != nullptr)
		{
			out << "\"" << this->typeName << "\"" << ", ";
		}
		else
		{
			out << "nullptr" << ", ";
		}

		if (this->methodName != nullptr)
		{
			out << "\"" << this->methodName << "\"" << ", ";
		}
		else
		{
			out << "nullptr" << ", ";
		}

		out << "{";
		for (size_t i = 0; i < this->parameterTypeList.size(); i++)
		{
			const char* v = this->parameterTypeList.at(i);
			v = v == nullptr ? "nullptr" : v;

			if (i != this->parameterTypeList.size() - 1)
			{
				out << "\"" << v << "\"" ", ";
			}
			else
			{
				out << "\"" << v << "\"";
			}
		}

		out << "}};";
		return out.str();
	}
}