#include "Websocket.hpp"
#include "../ClassFinder.hpp"
#include "../../util/HookingUtil.hpp"
#include "../../unity/Unity.hpp"

namespace Websocket
{
	Il2CppObject* WSManagerInstance = nullptr;

	Pointer<int(Il2CppObject* instance, MonoString* msgType, Il2CppObject* jsonMsg)> Send(
		"WebSocketManager",
		0x16
	);

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
		$RegisterHook(SendSocketMessage, Send.GetPointer());

		$RegisterHook(SocketSend, Il2CppUtils::GetMethodPointerByIndex(
			GetClass("WebsocketSex0"), 
			0x2
		));

		$RegisterHook(SocketRecieve, Il2CppUtils::GetMethodPointerByIndex(
			GetClass("WebsocketSex0"), 
			0x1
		));
	}
}