#include "GameServer.h"

bool GameServer::OnClientConnect(std::shared_ptr<connection<GameMsg>> client)
{
	return true;
}

void GameServer::OnClientValidated(std::shared_ptr<connection<GameMsg>> client)
{
	net::message<GameMsg> msg;
	msg.header.id = GameMsg::Client_Accepted;
	std::cout << "Sending accept" << std::endl;
	client->Send(msg);

}

void GameServer::OnClientDisconnect(std::shared_ptr<connection<GameMsg>> client)
{
	if (client)
	{
		if (m_PlayersMap.contains(client->GetID()))
		{
		}
		else
		{
			auto& pd = m_PlayersMap[client->GetID()];
			std::cout << "[UNGRACEFUL REMOVAL]: " << std::to_string(pd.Id) << std::endl;
			m_PlayersMap.erase(client->GetID());
			m_GarbageIds.push_back(client->GetID());
		}
	}
}

void GameServer::OnMessage(std::shared_ptr<connection<GameMsg>> client, net::message<GameMsg>& msg)
{
	if (!m_GarbageIds.empty())
	{
		for (auto& pid : m_GarbageIds)
		{
			net::message<GameMsg> msg;
			msg.header.id = GameMsg::Game_RemovePlayer;
			msg << pid;
			std::cout << "Removing: " << pid << std::endl;
			MessageAllClients(msg);
		}
		m_GarbageIds.clear();
	}

	switch (msg.header.id)
	{
	case GameMsg::Client_RegisterWithServer:
	{
		PlayerData pData;
		msg >> pData;
		pData.Id = client->GetID();
		
		net::message<GameMsg> msgSendID;
		msgSendID.header.id = GameMsg::Client_AssignID;
		msgSendID << pData.Id;
		MessageClient(client, msgSendID);

		net::message<GameMsg> msgAddPlayer;
		msgSendID.header.id = GameMsg::Game_AddPlayer;
		msgSendID << pData;
		MessageAllClients(msgAddPlayer);

		for (const auto& [pid, pData] : m_PlayersMap)
		{
			net::message<GameMsg> msgAddOtherPlayers;
			msgAddOtherPlayers.header.id = GameMsg::Game_AddPlayer;
			msgAddOtherPlayers << pData;
			MessageClient(client, msgAddOtherPlayers);
		}
		break;
	}
	case GameMsg::Client_UnregisterWithServer:
	{
		break;
	}
	case GameMsg::Game_UpdatePlayer:
	{
		MessageAllClients(msg, client);
		break;
	}
	}
}
