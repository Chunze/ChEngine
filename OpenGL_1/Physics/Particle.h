#pragma once
#include <glm/glm.hpp>

class Particle
{
public:
	Particle();

	Particle(float mass, glm::vec3 position, glm::vec3 volecity);

	float m_inverseMass;
	glm::vec3 m_position;
	glm::vec3 m_volecity;
};