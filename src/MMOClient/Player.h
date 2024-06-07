#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <SDL3/SDL.h>

#include "PlayerData.h"

class Player
{
public:
	glm::vec2 GetPos();
	void SetPos(glm::vec2 newPos);

	uint32_t GetId();
	PlayerData GetPlayerData();

	void Update();

private:
	uint32_t m_Id = 0;
	float m_Radius = 0;

	glm::vec2 m_Position;
	glm::vec2 m_Velocity;
};