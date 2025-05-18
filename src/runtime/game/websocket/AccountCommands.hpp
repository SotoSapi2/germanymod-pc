#pragma once
#include <iostream>

namespace AccountCommands
{
	void UnlockWeapon(int level, int rarity, size_t from, size_t to);

	void UnlockWeapon(int level, int rarity, const std::string& string);

	void UnlockRGBWeaponSet(int level, int rarity);

	void UnlockGoofyWeapons(int level, int rarity);

	void RemoveWeapon(size_t from, size_t to);

	void RemoveWeapon(const std::string& string);

	void UnlockGadget(int level);

	void UnlockGadget(const std::string& string, int level);

	void RemoveGadget();

	void RemoveGadget(const std::string& string);

	void UnlockModules(int amount);

	void UpgradeModules();

	void UnlockGraffities();

	void UnlockWeaponSkin(const std::string& string);

	void UnlockWeaponSkin(size_t from, size_t to);

	void UnlockDlcWeaponSkin();

	void AddParts();

	void AddFreeUpgrades(int amount);

	void GetVeteranBadge();

	void AddPlayerBuff(const std::string& buffName, int duration);

	void AddWinStat(int gamemodeEnum, int amount);

	void UpdateMonthlyStats(int kills, int deaths, int headshots, int killstreak, int winstreak);

	void AddMatchStat(bool isWinning);

	void CustomCape(const std::string& base64);
}