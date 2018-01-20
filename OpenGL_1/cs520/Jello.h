#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Particle.h"

class Jello
{
public:
	Jello();

	Jello(glm::vec3 position, float size);

	glm::vec3 m_position;
	float m_size = 7.0f;

	Particle m_particles[8][8][8];
};