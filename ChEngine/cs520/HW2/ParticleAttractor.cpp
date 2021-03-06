#include <iostream>

#include "ParticleAttractor.h"

ParticleAttractor::ParticleAttractor(GameContext* gameContext, World* world, glm::vec2 _position)
	: GameObject(gameContext, world),
	Position(_position)
{
	std::cout << "attractor created: x = " << Position.x << ", y = " << Position.y << std::endl;
}

void ParticleAttractor::SetPosition(int x, int y)
{
	Position.x = x;
	Position.y = y;
}

