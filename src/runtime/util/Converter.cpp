#include "Converter.hpp"
#include <codecvt>

namespace Convert
{
	typedef std::codecvt_utf8<wchar_t> convert_type;

	std::string ToString(const std::wstring& t_str)
	{
		static std::wstring_convert<convert_type, wchar_t> converter;
		return converter.to_bytes(t_str);
	}

	std::wstring ToWstring(const std::string& t_str)
	{
		static std::wstring_convert<convert_type, wchar_t> converter;
		return converter.from_bytes(t_str);
	}
}