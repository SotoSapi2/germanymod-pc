#include "WebsocketCore.hpp"
#include "MessageBuilder.hpp"
#include "../import/PointerFunctions.hpp"
#include "../Functions.hpp"
#include <optional>

namespace AccountCommands
{
	using nlohmann::json;

	void UnlockWeapon(int level, int rarity, size_t from, size_t to)
	{
		int cause = rand() % 158;
		cause = cause == 0 ? 1 : cause;

		WebsocketCore::ProgressUpdaterHelper helper;
		helper.IterateRegisterKey(OfferItemType::Weapon, CommandID::AddWeaponV17, from, to, [&](std::string key, int index) -> std::optional<json>
		{
			if (ContentKeyRegister::IsKeyBannable(key)) return std::nullopt;

			return json::object({
				{"i", index},
				{"oc", cause},
				{"u", json::object({
					{"l", level},
					{"r", rarity}
				})},
			});
		});

		helper.SendPackage();
	}

	void UnlockWeapon(int level, int rarity, const std::string& string)
	{
		int cause = rand() % 158;
		cause = cause == 0 ? 1 : cause;

		WebsocketCore::ProgressUpdaterHelper helper;
		helper.IterateRegisterKey(OfferItemType::Weapon, CommandID::AddWeaponV17, [&](std::string key, int index) -> std::optional<json::object_t>
		{
			if (ContentKeyRegister::IsKeyBannable(key) || string != key) return std::nullopt;

			return json::object({
				{"i", index},
				{"oc", cause},
				{"u", json::object({
					{"l", level},
					{"r", rarity}
				})},
			});
		});

		helper.SendPackage();
	}

	void UnlockRGBWeaponSet(int level, int rarity)
	{
		static std::array<std::string, 3> rgbWeaponsID =
		{
			"new_well_pistol",
			"fps_destroyer",
			"achieve_hunter"
		};

		WebsocketCore::ProgressUpdaterHelper helper;
		for (auto& v : rgbWeaponsID)
		{
			int index = ContentKeyRegister::GetItemIndex(
				ContentKeyRegister::GetInstance(),
				OfferItemType::Weapon,
				IL2CPP::String::Create(v)
			);

			helper.AddCommand(CommandID::AddWeaponV17, json::object({
				{"i", index},
				{"oc", 9999},
				{"u", json::object({
					{"l", level},
					{"r", rarity}
				})},
			}));
		}

		helper.SendPackage();
	}

	void UnlockGoofyWeapons(int level, int rarity)
	{
		static std::array<std::string, 9> goofyWeapons =
		{
			"iron_sword", "hound_weapon_one", "desert_fighter", 
			"locator", "advanced_scout_rifle_l", "harsh_punisher_l", 
			"manual_inferno_l", "offensive_rocket_system_l", "headhunter_pistols_l"
		};

		int cause = rand() % 158;
		cause = cause == 0 ? 1 : cause;

		WebsocketCore::ProgressUpdaterHelper helper;
		for (auto& v : goofyWeapons)
		{
			int index = ContentKeyRegister::GetItemIndex(
				ContentKeyRegister::GetInstance(),
				OfferItemType::Weapon,
				IL2CPP::String::Create(v)
			);

			helper.AddCommand(CommandID::AddWeaponV17, json::object({
				{"i", index},
				{"oc", cause},
				{"u", json::object({
					{"l", level},
					{"r", rarity}
				})},
			}));
		}

		helper.SendPackage();
	}

	void RemoveWeapon(size_t from, size_t to)
	{
		WebsocketCore::ProgressUpdaterHelper helper;
		helper.IterateRegisterKey(OfferItemType::Weapon, CommandID::RemoveWeapon, from, to, [&](std::string key, int index) -> std::optional<json::object_t>
		{
			return json::object({
				{"i", index}
			});
		});

		helper.SendPackage();
	}

	void RemoveWeapon(const std::string& string)
	{
		WebsocketCore::ProgressUpdaterHelper helper;
		helper.IterateRegisterKey(OfferItemType::Weapon, CommandID::RemoveWeapon, [&](std::string key, int index) -> std::optional<json::object_t>
		{
			if (key != string) return std::nullopt;

			return json::object({
				{"i", index}
			});
		});

		helper.SendPackage();
	}

	void UnlockGadget(int level)
	{
		WebsocketCore::ProgressUpdaterHelper helper;
		helper.IterateRegisterKey(OfferItemType::Gadget, CommandID::AddPurhasedGadget, [&](std::string key, int index) -> std::optional<json::object_t>
		{
			return json::object({
				{"g", index},
				{"i", level}
			});
		});

		helper.SendPackage();
	}

	void UnlockGadget(const std::string& string, int level)
	{
		WebsocketCore::ProgressUpdaterHelper helper;
		helper.IterateRegisterKey(OfferItemType::Gadget, CommandID::AddPurhasedGadget, [&](std::string key, int index) -> std::optional<json::object_t>
		{
			if (key != string) return std::nullopt;

			return json::object({
				{"g", index},
				{"i", level}
			});
		});

		helper.SendPackage();
	}

	void RemoveGadget()
	{
		WebsocketCore::ProgressUpdaterHelper helper;
		helper.IterateRegisterKey(OfferItemType::Gadget, CommandID::RemoveGadget, [&](std::string key, int index) -> std::optional<json::object_t>
		{
			return json::object({
				{"i", key}
			});
		});

		helper.SendPackage();
	}

	void RemoveGadget(const std::string& string)
	{
		WebsocketCore::ProgressUpdaterHelper helper;
		helper.IterateRegisterKey(OfferItemType::Gadget, CommandID::RemoveGadget, [&](std::string key, int index) -> std::optional<json::object_t>
		{
			if (key != string) return std::nullopt;

			return json::object({
				{"i", key}
			});
		});

		helper.SendPackage();
	}

	void UnlockModules(int amount)
	{
		WebsocketCore::ProgressUpdaterHelper helper;
		helper.IterateRegisterKey(OfferItemType::ModulePoint, CommandID::InventoryAddItemConsumable, [&](std::string key, int index) -> std::optional<json::object_t>
		{
			return json::object({
				{"t", OfferItemType::ModulePoint},
				{"i", index},
				{"c", amount}
			});
		});

		helper.SendPackage();
	}

	void UpgradeModules()
	{
		WebsocketCore::ProgressUpdaterHelper helper;
		helper.IterateRegisterKey(OfferItemType::Module, CommandID::ModuleInfoIncreaseUp, [&](std::string key, int index) -> std::optional<json::object_t>
		{
			return json::object({
				{"i", index},
			});
		});

		helper.SendPackage();
	}

	void UnlockGraffities()
	{
		WebsocketCore::ProgressUpdaterHelper helper;
		helper.IterateRegisterKey(OfferItemType::Graffiti, CommandID::InventoryAddItemSingle, [&](std::string key, int index) -> std::optional<json::object_t>
		{
			return json::object({
				{"i", index},
			});
		});

		helper.SendPackage();
	}

	void UnlockWeaponSkin(const std::string& string)
	{
		WebsocketCore::ProgressUpdaterHelper helper;
		helper.IterateRegisterKey(OfferItemType::WeaponSkin, CommandID::InventoryAddItemSingle, [&](std::string key, int index) -> std::optional<json::object_t>
		{
			if (ContentKeyRegister::IsKeyBannable(key) || string != key) return std::nullopt;

			return json::object({
				{"i", index},
			});
		});

		helper.SendPackage();
	}

	void UnlockWeaponSkin(size_t from, size_t to)
	{
		WebsocketCore::ProgressUpdaterHelper helper;
		helper.IterateRegisterKey(OfferItemType::WeaponSkin, CommandID::InventoryAddItemSingle, from, to, [&](std::string key, int index) -> std::optional<json::object_t>
		{
			if (ContentKeyRegister::IsKeyBannable(key)) return std::nullopt;

			return json::object({
				{"i", index},
			});
		});

		helper.SendPackage();
	}

	void UnlockDlcWeaponSkin()
	{
		WebsocketCore::ISocketHelper helper;
		helper.eventType = "update_progress";

		ContentKeyRegister::IterateKeyRegister(OfferItemType::WeaponSkin, [&](IL2CPP::String* key, int index)
		{
			if (!ContentKeyRegister::IsKeyBannable(key)) return std::nullopt;

			helper.package = json::object({
				{ "id", CommandID::InventoryAddItemSingle },
				{ "p", json::object({
					{ "i", index },
					{ "ca", 153 }
				})}
			});

			helper.SendPackage();
		});
	}

	void AddParts()
	{
		WebsocketCore::ProgressUpdaterHelper helper;
		helper.IterateRegisterKey(OfferItemType::Weapon, CommandID::AddDetails, [&](std::string key, int index) -> std::optional<json::object_t>
		{
			return json::object({
				{"i", index},
				{"d", 250}
			});
		});

		helper.IterateRegisterKey(OfferItemType::Royale, CommandID::AddDetails, [&](std::string key, int index) -> std::optional<json::object_t>
		{
			return json::object({
				{"i", index},
				{"d", 250}
			});
		});

		helper.SendPackage();
	}

	void AddFreeUpgrades(int amount)
	{
		WebsocketCore::ProgressUpdaterHelper helper;
		helper.IterateRegisterKey(OfferItemType::FreeUpgrade, CommandID::InventoryAddItemSingle, [&](std::string key, int index) -> std::optional<json::object_t>
		{
			return json::object({
				{"t", OfferItemType::FreeUpgrade},
				{"i", index},
				{"c", amount}
			});
		});

		helper.SendPackage();
	}

	void GetVeteranBadge()
	{
		WebsocketCore::ProgressUpdaterHelper helper;
		helper.IterateRegisterKey(OfferItemType::Achievement, CommandID::InventoryAddItemSingle, [&](std::string key, int index) -> std::optional<json::object_t>
		{
			if (key != "AchievementTimeInGame") std::nullopt;

			return json::object({
				{"i", index},	
			});
		});

		helper.SendPackage();
	}

	void AddPlayerBuff(const std::string& buffName, int duration)
	{
		auto currentTime = std::chrono::system_clock::now();
		std::time_t currentTimeInSeconds = std::chrono::system_clock::to_time_t(currentTime);

		WebsocketCore::ProgressUpdaterHelper helper;

		int index = ContentKeyRegister::GetItemIndex(
			ContentKeyRegister::GetInstance(),
			OfferItemType::PlayerBuff,
			IL2CPP::String::Create(buffName)
		);

		if (index == 0) return;
		helper.AddCommand(CommandID::InventoryAddItemExpiry, json::object({
			{"t", OfferItemType::PlayerBuff},
			{"i", index},
			{"s", duration},
			{"st", currentTimeInSeconds}
		}));
		helper.SendPackage();
	}

	void AddWinStat(int gamemodeEnum, int amount)
	{
		for (size_t i = 0; i < amount; i++)
		{
			int kill = rand() % 40;
			int death = rand() % 40;

			json out = json::object({
				{"kill_cnt", kill},
				{"death_cnt", death},
				{"kill_cnt_month", kill},
				{"death_cnt_month", death},
				{"accuracy", 1},
				{"mode", gamemodeEnum},
				{"rating", 0},
				{"league_id", 0}
			});

			WebsocketCore::QueuePackage("update_player", out);
		}
	}

	void UpdateMonthlyStats(int kills, int deaths, int headshots, int killstreak, int winstreak)
	{
		auto currentTime = std::chrono::system_clock::now();
		std::time_t currentTimeInSeconds = std::chrono::system_clock::to_time_t(currentTime);
		int timestampInSeconds = static_cast<int>(currentTimeInSeconds);
		std::tm* localTime = std::localtime(&currentTimeInSeconds);
		char dateStr[11];
		std::strftime(dateStr, sizeof(dateStr), "%m-%d-%Y", localTime);

		WebsocketCore::ProgressUpdaterHelper helper;
		helper.AddCommand(CommandID::UpdatePlayerStatsV2, json::object({
			{ "k", kills },
			{ "d", deaths },
			{ "w", true },
			{ "hds", headshots },
			{ "h", headshots },
			{ "s", killstreak },
			{ "ks", killstreak },
			{ "c", false },
			{ "tw", winstreak },
			{ "sw", winstreak },
			{ "sid", "" },
			{ "dk", dateStr }
		}));

		helper.AddCommand(CommandID::BattleStatisticEndMatch, json::object({
			{ "u", timestampInSeconds },
			{ "w", 10000 },
			{ "sid", "" },
		}));

		helper.SendPackage();
	}

	void AddMatchStat(bool isWinning)
	{
		auto currentTime = std::chrono::system_clock::now();
		std::time_t currentTimeInSeconds = std::chrono::system_clock::to_time_t(currentTime);
		int timestampInSeconds = static_cast<int>(currentTimeInSeconds);
		std::tm* localTime = std::localtime(&currentTimeInSeconds);
		char dateStr[11];
		std::strftime(dateStr, sizeof(dateStr), "%m-%d-%Y", localTime);

		WebsocketCore::ProgressUpdaterHelper helper;
		helper.AddCommand(CommandID::UpdatePlayerStatsV2, json::object({
			{ "k", 0 },
			{ "d", 0 },
			{ "w", isWinning },
			{ "hds", 0 },
			{ "h", 0 },
			{ "s", 0 },
			{ "ks", 0 },
			{ "c", false },
			{ "tw", 0 },
			{ "sw", 0 },
			{ "sid", "" },
			{ "dk", dateStr }
		}));

		helper.AddCommand(CommandID::BattleStatisticEndMatch, json::object({
			{ "u", timestampInSeconds },
			{ "w", 10000 },
			{ "sid", "" },
		}));

		helper.SendPackage();
	}

	void CustomCape(const std::string& base64)
	{
		WebsocketCore::ProgressUpdaterHelper helper;
		helper.AddCommand(CommandID::UpdateCustomSkin, json::object({
			{"i", -1},
			{"c", base64}
		}));
		helper.SendPackage();
	}
}