#include "Websocket.hpp"
#include <json.hpp>
#include <imgui.h>
#include <queue>

#include "../util/HookingUtil.hpp"
#include "../unity/Unity.hpp"
#include "../framework/UIManager.hpp"

#include "data/ClassFinder.hpp"
#include "data/PointerFunctions.hpp"
#include "Functions.hpp"
#include "PixelTime.hpp"

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

	json BuildSnapshot(const json& arr)
	{
		std::string hexId = RandHex();

		json outJson = json::object({
			{"i", hexId},
			{"id", CommandID::Snapshot},
			{"p", {{"c", arr}}}
		});

		return outJson;
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
}

using namespace MessageBuilder;
namespace Websocket
{
	using nlohmann::json;

	bool test = false;
	bool logWebsocket = false;
	Il2CppObject* WSManagerInstance = nullptr;
	std::queue<json> commandQueue;

	void Queue(const std::string& msgType, const json& json_)
	{
		commandQueue.push(json_);
		WebSocketManager::SendEvent(WSManagerInstance, MonoString::Create(msgType.c_str()));
	}

	void SaveProgress(const json& arr)
	{
		Queue("update_progress", MessageBuilder::BuildSnapshot(arr));
	}

	void SaveProgressNoSnapshot(CommandID cmdId, const json& json_)
	{
		Queue("update_progress", BuildCommand(cmdId, json_));
	}

	void Reload()
	{
		Queue("update_progress", json::object({}));
	}

	void AddCurrencyTest()
	{
		json command = json::object({
			{"c", "GemsCurrency"},
			{"v", 1000},
			{"ca", 16}
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

		SaveProgressNoSnapshot(CommandID::InventoryAddItemSingle, command);
	}

	void AddAllDlcTest()
	{
		ContentKeyRegister::IterateKeyRegister(OfferItemType::WeaponSkin, [&](MonoString* key, int index)
		{
			if (!ContentKeyRegister::IsKeyBannable(key)) return;

			json command = json::object({
				{"i", index},
				{"ca", 153}
			});

			SaveProgressNoSnapshot(CommandID::InventoryAddItemSingle, command);
		});
	}

	void UIUpdate()
	{
		ImGui::Checkbox("Log Websocket", &logWebsocket);

		if (ImGui::Button("Add Currency test"))
		{
			AddCurrencyTest();
		}

		if (ImGui::Button("DLC Skins test"))
		{
			AddSkinTest();
		}

		if (ImGui::Button("Add all DLC Skins test"))
		{
			AddAllDlcTest();
		}

		if (ImGui::Button("Reload"))
		{
			Reload();
		}
	}

	$Hook(int, SendSocketMessage, (Il2CppObject* _this, MonoString* messageType, Il2CppObject* jsonObject))
	{
		if (WSManagerInstance == nullptr)
		{
			WSManagerInstance = _this;
		}

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
				json outJson = json::parse(str->ToUtf8());

				if (outJson.contains("sid"))
				{
					sid = outJson["sid"];
				}
				if (outJson.contains("req_id"))
				{
					reqId = outJson["req_id"];
				}

				if (!commandQueue.empty())
				{
					json outJson = commandQueue.front();
					outJson["req_id"] = reqId;
					outJson["sid"] = sid;

					if (logWebsocket)
					{
						LOG_NOTAG("[Send (Custom) | %s]\n%s", command.c_str(), outJson.dump().c_str());
					}

					commandQueue.pop();
					return $CallOrig(SocketSend, _this, MonoString::Create(outJson.dump())->ToMonoArray(), idfk);
				}

				if (logWebsocket)
				{
					LOG_NOTAG("[Send | %s]\n%s", command.c_str(), str->ToUtf8());
				}
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
			std::string sid = "";
			int reqId = 0;
			MonoString* str = MonoString::FromMonoArray(buffer);

			if (*buffer->vector == '{')
			{
				json outJson = json::parse(str->ToUtf8());

				if (outJson.contains("sid"))
				{
					sid = outJson["sid"];
				}
				if (outJson.contains("req_id"))
				{
					reqId = outJson["req_id"];
				}

				if (logWebsocket)
				{
					LOG_NOTAG("[%s | %s]\n%s", "Recieve", command.c_str(), str->ToUtf8());
				}
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