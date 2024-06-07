#include "Player.h"

glm::vec2 Player::GetPos()
{
	return m_Position;
}

void Player::SetPos(glm::vec2 newPos)
{
	m_Position = newPos;
}

uint32_t Player::GetId()
{
	return m_Id;
}

PlayerData Player::GetPlayerData()
{
	return PlayerData(m_Id, m_Radius, m_Position, m_Velocity);
}

void Player::Update()
{

}

