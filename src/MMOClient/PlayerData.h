#pragma once

#include <glm/glm.hpp>

struct PlayerData
{
	uint32_t Id = 0;
	float Radius = 0;

	glm::vec2 Position;
	glm::vec2 Velocity;
};