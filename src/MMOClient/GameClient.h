#pragma once

#include <unordered_map>
#include <noodle_net.h>
#include "Messages.h"
#include "PlayerData.h"
#include "Player.h"
#include "Game.h"

class GameClient : public net::client_interface<GameMsg>
{
public:
	bool OnUserCreate();

	void Update();

	void HandleMessage();
	void HandleAccept();
	template<typename T>
	void HandleAssign(net::message<T> msg);
	template<typename T>
	void HandleAdd(net::message<T> msg);
	template<typename T>
	void HandleRemove(net::message<T> msg);
	template<typename T>
	void HandleUpdate(net::message<T> msg);

private:
	Player m_Player;
	Game m_Game;

	bool m_WaitingForConnection = true;
};