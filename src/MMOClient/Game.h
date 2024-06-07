#pragma once

#include <unordered_map>
#include "PlayerData.h"

class Game
{
public:
	void AddUpdatePlayer(PlayerData&& newPlayerData);
	void RemovePlayer(uint32_t playerId);

private:
	std::unordered_map<uint32_t, PlayerData> m_Players;
};