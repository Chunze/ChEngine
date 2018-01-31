#include "Particle.h"

Particle::Particle(float mass, glm::vec3 position, glm::vec3 volecity)
	: m_position(position),
	  m_volecity(volecity)
{
	if (mass <= 0)
	{
		m_inverseMass = INFINITY;
	}
	else
	{
		m_inverseMass = 1.0f / mass;
	}


}

Particle::Particle()
{
	Particle(1.0f, glm::vec3(0.0f), glm::vec3(0.0f));
}

void Particle::addForce(const glm::vec3 &force)
{
	forceAccum += force;
}
