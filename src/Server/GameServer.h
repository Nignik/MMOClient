#pragma once

#include <iostream>
#include <noodle_net.h>

#include "../MMOClient/PlayerData.h"

using net::connection;

enum class GameMsg : uint32_t
{
	Server_GetStatus,
	Server_GetPing,

	Client_Accepted,
	Client_AssignID,
	Client_RegisterWithServer,
	Client_UnregisterWithServer,

	Game_AddPlayer,
	Game_RemovePlayer,
	Game_UpdatePlayer,
};

class GameServer : public net::server_interface<GameMsg>
{
public:
	GameServer(uint16_t nPort) : server_interface<GameMsg>(nPort)
	{

	}

protected:
	virtual bool OnClientConnect(std::shared_ptr<connection<GameMsg>> client);
	virtual void OnClientValidated(std::shared_ptr<connection<GameMsg>> client);
	virtual void OnClientDisconnect(std::shared_ptr<connection<GameMsg>> client);
	virtual void OnMessage(std::shared_ptr<connection<GameMsg>> client, net::message<GameMsg>& msg);

private:
	std::unordered_map<uint32_t, PlayerData> m_PlayersMap;
	std::vector<uint32_t> m_GarbageIds;
};

