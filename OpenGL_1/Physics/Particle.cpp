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

void Particle::Integrate(float Delta, int Mode)
{
	if (Mode == 0)
	{
		m_position = m_volecity * Delta;
		m_volecity = m_acceleration * Delta;
		m_acceleration = forceAccum * m_inverseMass;

		m_volecity *= powf(damping, Delta);
	}
	else if (Mode == 1)
	{
		glm::vec3 f1p, f2p, f3p, f4p;
		glm::vec4 f1v, f2v, f3v, f4v;


	}

	ClearForce();
}

void Particle::ClearForce()
{
	forceAccum.x = 0.0f;
	forceAccum.y = 0.0f;
	forceAccum.z = 0.0f;
}
