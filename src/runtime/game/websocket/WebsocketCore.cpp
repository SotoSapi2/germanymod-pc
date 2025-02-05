#include "WebsocketCore.hpp"
#include <IL2CPP.hpp>
#include <queue>
#include <Logger.hpp>

#include "MessageBuilder.hpp"
#include "../import/PointerFunctions.hpp"
#include "../../util/HookingUtil.hpp"

namespace WebsocketCore
{
	using nlohmann::json;

	bool logWebsocket = false;
	IL2CPP::Object* WSManagerInstance = nullptr;
	std::vector<json> commandQueue;

	void Queue(const std::string& msgType, const json& json_)
	{
		commandQueue.push_back(json_);
		WebSocketManager::SendEvent(WSManagerInstance, IL2CPP::String::Create(msgType.c_str()));
	}

	void SaveProgress(const json& arr)
	{
		Queue("update_progress", MessageBuilder::BuildSnapshot(arr));
	}

	void SaveProgressNoSnapshot(CommandID cmdId, const json& json_)
	{
		Queue("update_progress", MessageBuilder::BuildCommand(cmdId, json_));
	}

	void Reload()
	{
		Queue("update_progress", json::object({}));
	}

	$Hook(int, SendSocketMessage, (IL2CPP::Object* _this, IL2CPP::String* messageType, IL2CPP::Object* jsonObject))
	{
		if (WSManagerInstance == nullptr)
		{
			WSManagerInstance = _this;
		}

		return $CallOrig(SendSocketMessage, _this, messageType, jsonObject);
	}

	$Hook(IL2CPP::Array<char>*, SocketReceive, (IL2CPP::Object* _this, IL2CPP::Array<char>* rawBuffer))
	{
		using namespace nlohmann;
		IL2CPP::Array<char>* buffer = $CallOrig(SocketReceive, _this, rawBuffer);

		if (buffer == nullptr)
		{
			return buffer;
		}

		static std::string command;
		std::string sid;
		int reqId;
		std::string data{ buffer->GetVectorPointer(), buffer->GetVectorPointer() + buffer->GetLength() };

		if (buffer->Get(0) == '{')
		{
			json parsedData = json::parse(data);
		}
		else
		{
			command = data;
		}

		return buffer;
	}

	$Hook(IL2CPP::Array<char>*, SocketSend, (IL2CPP::Object* _this, IL2CPP::Array<char>* buffer, bool idfk))
	{
		using namespace nlohmann;
		if (buffer == nullptr)
		{
			return $CallOrig(SocketSend, _this, buffer, idfk);
		}

		static std::string command = "";
		std::string sid = "";
		int reqId = -1;
		std::string data{ buffer->GetVectorPointer(), buffer->GetVectorPointer() + buffer->GetLength() };

		if (buffer->Get(0) == '{')
		{
			json parsedData = json::parse(data);

			if (parsedData.contains("sid"))
			{
				sid = parsedData["sid"];
			}
			if (parsedData.contains("req_id"))
			{
				reqId = parsedData["req_id"];
			}

			if (!commandQueue.empty())
			{
				for (auto it = commandQueue.begin(); it < commandQueue.end(); ++it)
				{
					if (it->at("req_id") != reqId) continue;
					commandQueue.erase(it);

					return $CallOrig(
						SocketSend,
						_this,
						Encoding::GetBytes(
							Encoding::GetUTF8(),
							IL2CPP::String::Create(it->dump())
						),
						idfk
					);
				}
			}

			return $CallOrig(SocketSend, 
				_this, 
				Encoding::GetBytes(
					Encoding::GetUTF8(), 
					IL2CPP::String::Create(parsedData.dump())
				), 
				idfk
			);
		}
		else
		{
			command = data;
		}

		return $CallOrig(SocketSend, _this, buffer, idfk);
	}


	void INIT()
	{
		using namespace IL2CPP::ClassMapping;

		$RegisterHook(
			SendSocketMessage, 
			WebSocketManager::Send.GetPointer()
		);

		$RegisterHook(
			SocketSend, 
			GetClass("AesFacade")->GetMethod(0x2)
		);

		$RegisterHook(
			SocketReceive,
			GetClass("AesFacade")->GetMethod(0x1)
		);
	}
}