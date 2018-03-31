#pragma once
#include "GameObject.h"
#include "glm.h"

class ParticleAttractor : public GameObject
{
public:
	ParticleAttractor(GameContext* gameContext, World* world, glm::vec2 _position);

	glm::vec2 Position;

	bool bIsActive = true;
};