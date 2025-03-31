#include "MessageBuilder.hpp"

namespace MessageBuilder
{
	using nlohmann::json;
	std::string version("24.8.0");

	std::string RandHex(int hexLength)
	{
		std::stringstream ss;
		for (int i = 0; i < hexLength; i++)
		{
			ss << std::hex << std::setw(2) << std::setfill('0') << rand() % 255;
		}

		return ss.str();
	}

	json BuildSnapshot(const json& arr)
	{
		return 
		{
			{"h", json::object()},
			{"i", RandHex()},
			{"id", CommandID::Snapshot},
			{"p", {{"c", arr}}}
		};
	}

	json BuildCommand(CommandID cmdId, const json& body, const std::vector<int>& u)
	{
		return
		{
			{"id", cmdId},
			{"ci", RandHex()},
			{"p", body},
			{"h", json(json::value_t::object)},
			{"v", version},
			{"u", u}
		};
	}
}