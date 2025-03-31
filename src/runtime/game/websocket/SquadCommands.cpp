// in_squad
// {"clanwar_mode":0,"invite_number":1,"name":"the guy called joe","protocol":"ios21.5.0mm_v2ios","receiver_id":"260430910","req_id":143,"sid":"e8e76e32bbe04560ab598e5fd02197aa","siege_mode":0,"squad_name":"1851e437-5b14-40a7-9d0e-70e805ab8294","test_mode":0}
#include "SquadCommands.hpp"
#include "WebsocketCore.hpp"

namespace SquadCommands
{
	using nlohmann::json;

	void InSquad(const std::string& squadHash)
	{
		json payload = json::object({
			{ "squad_id", squadHash }
		});

		WebsocketCore::QueuePackage("in_squad", payload);
	}

	void SendSquadInvite(const std::string& squadHash, const std::string& name, const std::string& receiverId)
	{
		static int inviteNumber = 0;

		json payload = json::object({
			{ "name", name },
			{ "receiver_id", receiverId },
			{ "invite_number", inviteNumber },
			{ "protocol", "ios21.5.0mm_v2ios"},
			{ "squad_name", squadHash},
			{ "test_mode", 0 },
			{ "clanwar_mode", 0 },
			{ "siege_mode", 0},
		});

		WebsocketCore::QueuePackage("send_squad_invite", payload);	
		inviteNumber++;
	}
}