#pragma once
#include <vector>
#include <string>

namespace IL2CPP
{
	typedef std::vector<const char*> FieldPattern;

	struct SignaturePattern
	{
		const char* modifier = nullptr;
		const char* typeName = nullptr;
		const char* methodName = nullptr;
		std::vector<const char*> parameterTypeList;

		SignaturePattern(const char* modifier, const char* typeName, const char* methodName, std::vector<const char*> parameterTypeList);

		std::string ToString() const;
	};
}