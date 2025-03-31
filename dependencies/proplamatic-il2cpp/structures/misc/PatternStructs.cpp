#include "PatternStructs.hpp"
#include <sstream>

namespace IL2CPP
{


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
			const char* v = this->parameterTypeList[i];
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