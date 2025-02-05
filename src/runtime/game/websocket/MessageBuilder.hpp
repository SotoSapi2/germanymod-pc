#pragma once
#include <json.hpp>
#include "../import/Structs.hpp"

namespace MessageBuilder
{
	using nlohmann::json;
	std::string RandHex(int hexLength = 4);

	json BuildSnapshot(const json& arr);

	json BuildCommand(CommandID cmdId, const json& body, const std::vector<int>& u = { 140, 6 });
}