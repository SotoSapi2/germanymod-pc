#pragma once
#include <json.hpp>
#include <functional>
#include "../import/Structs.hpp"

namespace WebsocketCore
{
	using nlohmann::json;
	extern bool logWebsocket;
	extern IL2CPP::Object* WSManagerInstance;

	void QueuePackage(const std::string& msgType, const json& package, const std::function<void(json& response)>& onReceive = nullptr);

	inline int PendingPackageCount();

	inline int PendingReceiverCount();

	void Reload();

	void INIT();

	class ISocketHelper
	{
		public:
		std::string eventType;
		json package;

		public:
		virtual void SendPackage(const std::function<void(json& response)>& onRecieve = nullptr);
	};

	class ProgressUpdaterHelper : public ISocketHelper
	{
		public:
		ProgressUpdaterHelper();

		virtual void IterateRegisterKey(OfferItemType itemType, CommandID cmdId, const std::function<std::optional<json::object_t>(std::string, int)>& iterator);

		virtual void IterateRegisterKey(OfferItemType itemType, CommandID cmdId, size_t from, size_t to, const std::function<std::optional<json::object_t>(std::string, int)>& iterator);

		virtual void AddCommand(CommandID cmdId, const json& json_);

		void SendPackage(const std::function<void(json& response)>& onRecieve = nullptr) override;
	};
}