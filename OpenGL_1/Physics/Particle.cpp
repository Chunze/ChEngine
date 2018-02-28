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

void Particle::Integrate_Euler(float Delta)
{
	m_acceleration = forceAccum * m_inverseMass;

	if (bUseGravite)
	{
		m_acceleration += m_gravity;
	}

	m_volecity += m_acceleration * Delta;

	m_position += m_volecity * Delta;
	
	ClearForce();
}

void Particle::Integrate_Rk4(float Delta, int step)
{
	glm::vec3 f1p, f2p, f3p, f4p;
	glm::vec3 f1v, f2v, f3v, f4v;

	switch (step)
	{
	case 1:
		m_position = 0.5f * m_volecity * Delta + m_position;
		m_volecity = 0.5f * m_acceleration * Delta + m_volecity;
		break;
	case 2:

		break;
	case 3:

		break;
	case 4:

		break;
	default:
		break;
	}
	
}

void Particle::ClearForce()
{
	forceAccum.x = 0.0f;
	forceAccum.y = 0.0f;
	forceAccum.z = 0.0f;
}
