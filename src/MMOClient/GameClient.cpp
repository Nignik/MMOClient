#include "GameClient.h"


bool GameClient::OnUserCreate()
{
	if (Connect("127.0.0.1", 60000))
	{
		return true;
	}

	return false;
}

void GameClient::Update()
{
	if (IsConnected())
	{
		while (!Incoming().empty())
		{
			std::cout << "handling message" << std::endl;
			HandleMessage();
		}
	}

	if (m_WaitingForConnection)
	{
		return;
	}
}

void GameClient::HandleMessage()
{
	auto msg = Incoming().pop_front().msg;

	switch (msg.header.id)
	{
	case(GameMsg::Client_Accepted):
	{
		HandleAccept();
		break;
	}

	case(GameMsg::Client_AssignID):
	{
		HandleAssign(msg);
		break;
	}

	case(GameMsg::Game_AddPlayer):
	{
		HandleAdd(msg);
		break;
	}

	case(GameMsg::Game_RemovePlayer):
	{
		HandleRemove(msg);
		break;
	}

	case(GameMsg::Game_UpdatePlayer):
	{
		HandleUpdate(msg);
		break;
	}
	}
}

void GameClient::HandleAccept()
{
	std::cout << "Server accepted client - you're in!\n";
	net::message<GameMsg> msg;
	msg.header.id = GameMsg::Client_RegisterWithServer;
	m_Player.SetPos(glm::vec2(3.0f, 3.0f));
	msg << m_Player.GetPlayerData();
	Send(msg);
}

template<typename T>
void GameClient::HandleAssign(net::message<T> msg)
{
	uint32_t newId;
	msg >> newId;
	std::cout << "Assigned Client ID = " << newId << "\n";
}

template<typename T>
void GameClient::HandleAdd(net::message<T> msg)
{
	std::cout << "Tried to add player" << std::endl;
	PlayerData newPlayerData;
	msg >> newPlayerData;
	uint32_t newId = newPlayerData.Id;
	m_Game.AddUpdatePlayer(std::move(newPlayerData));

	if (newId == m_Player.GetId())
	{
		// Now we exist in game world
		m_WaitingForConnection = false;
	}
}

template<typename T>
void GameClient::HandleRemove(net::message<T> msg)
{
	uint32_t removalId = 0;
	msg >> removalId;
	m_Game.RemovePlayer(removalId);
}

template<typename T>
void GameClient::HandleUpdate(net::message<T> msg)
{
	PlayerData newData;
	msg >> newData;
	m_Game.AddUpdatePlayer(std::move(newData));
}

