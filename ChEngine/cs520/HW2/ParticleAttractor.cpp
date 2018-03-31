#include <ostream>

#include "ParticleAttractor.h"

ParticleAttractor::ParticleAttractor(GameContext* gameContext, World* world, glm::vec2 _position)
	: GameObject(gameContext, world),
	Position(_position)
{
	std::cout << "attractor created: x = " << Position.x << ", y = " << Position.y << std::endl;
}

