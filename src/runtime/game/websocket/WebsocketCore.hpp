#pragma once
#include <json.hpp>
#include "../import/Structs.hpp"

namespace WebsocketCore
{
	using nlohmann::json;
	void Queue(const std::string& msgType, const json& json_);

	void SaveProgress(const json& arr);

	void SaveProgressNoSnapshot(CommandID cmdId, const json& json_);

	void Reload();

	void INIT();
}