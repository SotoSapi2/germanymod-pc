#pragma once
#include <iostream>

namespace ClanCommands
{
	void UnlockAllClanPlaceable(std::string clanId, int amount);
	void UnlockClanPlaceable(std::string clanId, const std::string& targetKey, int amount);
	void PromoteClan(std::string clanId);
}