#include "WebsocketCore.hpp"
#include "MessageBuilder.hpp"
#include "../import/PointerFunctions.hpp"
#include "../Functions.hpp"
#include <optional>

namespace ClanCommands
{
	using nlohmann::json;

	void UnlockAllClanPlaceable(std::string clanId, int amount)
	{
		int counter = 1;
		WebsocketCore::ProgressUpdaterHelper helper;
		helper.IterateRegisterKey(OfferItemType::ClanPlaceable, CommandID::CreateClanItems, [&](std::string key, int index)
		{
			json item = json::object({
				{ "id", std::format("{0}.{1}.{2}", clanId, GetTickCount64(), counter)},
				{ "clan", clanId },
				{ "item_index", index },
				{ "count", amount }
			});

			json data = json::object({
				{ "items", json::array({item}) }
			});


			counter++;
			return data;
		});

		helper.SendPackage();
	}

	void UnlockClanPlaceable(std::string clanId, const std::string& targetKey, int amount)
	{
		int counter = 1;
		WebsocketCore::ProgressUpdaterHelper helper;
		helper.IterateRegisterKey(OfferItemType::ClanPlaceable, CommandID::CreateClanItems, [&](std::string key, int index) -> std::optional<json::object_t>
		{
			if (key != targetKey) return std::nullopt;

			json item = json::object({
				{ "id", std::format("{0}.{1}.{2}", clanId, GetTickCount64(), counter)},
				{ "clan", clanId },
				{ "item_index", index },
				{ "count", amount }
			});

			json data = json::object({
				{ "items", json::array({item}) }
			});

			counter++;
			return data;
		});

		helper.SendPackage();
	}

	void PromoteClan(std::string clanId)
	{
		WebsocketCore::ProgressUpdaterHelper helper;
		helper.AddCommand(CommandID::UpdateUtilityDictionaryValue, json::object({
			{"1", json::object({
				{"138", clanId}
			})}
		}));

		helper.SendPackage();
	}
}