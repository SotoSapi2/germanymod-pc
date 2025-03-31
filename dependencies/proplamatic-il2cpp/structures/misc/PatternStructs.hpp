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
		const const char* methodName = nullptr;
		const std::vector<const char*> parameterTypeList{};

		SignaturePattern(const char* modifier, const char* typeName, const char* methodName, std::vector<const char*> parameterTypeList) :
			modifier(modifier),
			typeName(typeName),
			methodName(methodName),
			parameterTypeList(parameterTypeList)
		{}

		std::string ToString() const;
	};
}