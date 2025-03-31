#pragma once
#include <string>

namespace Convert
{
	std::string ToString(const std::wstring& t_str);
	std::wstring ToWstring(const std::string& t_str);
	std::string LowercaseString(std::string copy);
}

