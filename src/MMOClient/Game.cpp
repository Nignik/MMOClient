#include "Game.h"

void Game::AddUpdatePlayer(PlayerData&& newPlayerData)
{
	auto id = newPlayerData.Id;
	m_Players.insert_or_assign(id, std::move(newPlayerData));
}

void Game::RemovePlayer(uint32_t playerId)
{
	m_Players.erase(playerId);
}
