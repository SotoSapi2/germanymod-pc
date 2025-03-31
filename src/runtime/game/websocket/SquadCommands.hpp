#pragma once
#include <string>

namespace SquadCommands
{
	void InSquad(const std::string& squadHash);
	void SendSquadInvite(const std::string& squadHash, const std::string& name, const std::string& receiverId);
}