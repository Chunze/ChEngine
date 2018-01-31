#pragma once
#include <vector>
#include "glm.h"
#include "GameObject.h"
#include "Particle.h"


class Jello : public GameObject
{
public:

	Jello(GameContext gameContext, World* world, glm::vec3 position = glm::vec3(0.0f), float size = 7.0f);

	glm::vec3 m_position;
	float m_size;
	float m_mass;

	Particle m_particles[8][8][8];

	float m_kElastic; // Hook's elasticity coefficient for all springs except collision springs
	float m_dElastic; // Damping coefficient for all springs except collision springs
	float m_kCollision; // Hook's elasticity coefficient for collision springs
	float m_dCollision; // Damping coefficient collision springs

	virtual void DrawObject(int Mode);

protected:
	
	std::vector<float> GetVertices();
};