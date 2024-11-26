#include "Websocket.hpp"
#include <json.hpp>
#include <imgui.h>

#include "../util/HookingUtil.hpp"
#include "../unity/Unity.hpp"
#include "../framework/UIManager.hpp"

#include "data/ClassFinder.hpp"
#include "data/PointerFunctions.hpp"

namespace MessageBuilder
{
	using nlohmann::json;
	std::string version("24.8.0");

	std::string RandHex(int hexLength = 4)
	{
		std::stringstream ss;
		for (int i = 0; i < hexLength; i++)
		{
			ss << std::hex << std::setw(2) << std::setfill('0') << rand() % 255;
		}

		return ss.str();
	}

	json BuildBase(const json& body, std::string& hexId)
	{
		json outJson = {
			{"d", body},
			{"i", hexId}
		};

		return outJson;
	}

	json BuildSnapshot(const json& arr)
	{
		std::string hexId = RandHex();

		json outJson = {
			{"i", hexId},
			{"id", CommandID::Snapshot},
			{"p", {{"c", arr}}}
		};

		return BuildBase(outJson, hexId);
	}

	json BuildCommand(CommandID cmdId, json body, std::vector<int> u = { 140, 6 })
	{
		std::string hexId = RandHex();

		return
		{
			{"id", cmdId},
			{"ci", hexId},
			{"p", body},
			{"h", json(json::value_t::object)},
			{"v", version},
			{"u", u}
		};
	}

	Il2CppObject* BakeJson(const json& json_)
	{
		std::string jsonStr = json_.dump();
		MonoString* str = MonoString::Create(jsonStr.c_str());

		return MiniJson::Deserialize(str);
	}
}

using namespace MessageBuilder;
namespace Websocket
{
	using nlohmann::json;

	bool logWebsocket = false;
	Il2CppObject* WSManagerInstance = nullptr;

	void SaveProgress(const json& json_)
	{
		Il2CppObject* instance = ProgressUpdater::GetInstance();
		Il2CppObject* bakedJson = BakeJson(BuildSnapshot(json_));

		ProgressUpdater::SaveProgress(instance, bakedJson);
	}

	void Reload()
	{
		json command = json::object({ {"RELOAD", 1} });
		json out = json::array({ BuildCommand((CommandID)rand(), command) });
		SaveProgress(out);
	}

	void AddCurrencyTest()
	{
		json command = json::object({
			{"c", "GemsCurrency_1"},
			{"v", 1000},
			{"ca", 6}
			});

		json out = json::array({
			BuildCommand(CommandID::AddCurrency, command),
			});

		SaveProgress(out);
	}

	void AddSkinTest()
	{
		json command = json::object({
			{"i", 496014},
			{"ca", 153}
			});

		json out = json::array({
			BuildCommand(CommandID::InventoryAddItemSingle, command),
			});

		SaveProgress(out);
	}

	void UIUpdate()
	{
		ImGui::Checkbox("Log Websocket", &logWebsocket);

		if (ImGui::Button("Add Currency Test"))
		{
			AddCurrencyTest();
		}

		if (ImGui::Button("Test DLC Skins"))
		{
			AddSkinTest();
		}

		if (ImGui::Button("Reload"))
		{
			Reload();
		}
	}

	$Hook(int, SendSocketMessage, (Il2CppObject* _this, MonoString* messageType, Il2CppObject* jsonObject))
	{
		WSManagerInstance = _this;
		return $CallOrig(SendSocketMessage, _this, messageType, jsonObject);
	}

	$Hook(MonoArray<char>*, SocketSend, (Il2CppObject* _this, MonoArray<char>* buffer, bool idfk))
	{
		if (buffer)
		{
			static std::string command;
			std::string sid;
			int reqId;
			MonoString* str = MonoString::FromMonoArray(buffer);

			if (*buffer->vector == '{')
			{
				LOG_NOTAG("[%s | %s]\n%s", "Send", command.c_str(), str->ToUtf8());
			}
			else
			{
				command = str->ToUtf8String();
			}
		}

		return $CallOrig(SocketSend, _this, buffer, idfk);
	}

	$Hook(MonoArray<char>*, SocketRecieve, (Il2CppObject* _this, MonoArray<char>* rawBuffer))
	{
		auto buffer = $CallOrig(SocketRecieve, _this, rawBuffer);

		if (buffer)
		{
			static std::string command = "";
			std::string sid;
			int reqId = 0;
			MonoString* str = MonoString::FromMonoArray(buffer);

			if (*buffer->vector == '{')
			{
				LOG_NOTAG("[%s | %s]\n%s", "Recieve", command.c_str(), str->ToUtf8());
			}
			else
			{
				command = str->ToUtf8String();
			}
		}

		return buffer;
	}

	void INIT()
	{
		$RegisterHook(SendSocketMessage, WebSocketManager::Send.GetPointer());

		$RegisterHook(SocketSend, Il2CppUtils::GetMethodPointerByIndex(
			GetClass("WebsocketSex0"),
			0x2
		));

		$RegisterHook(SocketRecieve, Il2CppUtils::GetMethodPointerByIndex(
			GetClass("WebsocketSex0"),
			0x1
		));

		UIManager::RegisterUIUpdate(UIUpdate);
	}
}