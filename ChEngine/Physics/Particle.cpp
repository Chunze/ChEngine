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
	m_forceAccum += force;
}

void Particle::Integrate_Euler(float Delta)
{
	m_acceleration = m_forceAccum * m_inverseMass;

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
	if(m_forceAccum.x != 0.0f || m_forceAccum.y != 0.0f || m_forceAccum.z != 0.0f)
	{
		switch (step)
		{
		case 1:
			// saving position and volecity at the beginning of the frame
			m_prePosition = m_position;
			m_preVolecity = m_volecity;


			RK4_A_1 = m_forceAccum * m_inverseMass;
			RK4_V_1 = RK4_A_1 * Delta + m_preVolecity;
			m_volecity = RK4_V_1;
			m_position = m_prePosition + RK4_V_1 * Delta;
			ClearForce();
			break;
		case 2:
			RK4_A_2 = m_forceAccum * m_inverseMass;
			RK4_V_2 = RK4_A_2 * Delta * 0.5f + m_preVolecity;
			m_volecity = RK4_V_2;
			m_position = m_prePosition + RK4_V_2 * Delta * 0.5f;
			ClearForce();
			break;
		case 3:
			RK4_A_3 = m_forceAccum * m_inverseMass;
			RK4_V_3 = RK4_A_3 * Delta * 0.5f + m_preVolecity;
			m_volecity = RK4_V_3;
			m_position = m_prePosition + RK4_V_3 * Delta * 0.5f;
			ClearForce();
			break;
		case 4:
			RK4_A_4 = m_forceAccum * m_inverseMass;
			RK4_V_4 = RK4_A_4 * Delta + m_preVolecity;

			m_volecity = (RK4_V_1 + 2.0f * (RK4_V_2 + RK4_V_3) + RK4_V_4) * 0.166667f /* = 1 / 6 */;
			m_position = m_prePosition + m_volecity * Delta;
			ClearForce();
			break;
		default:
			break;
		}
	}
	
}

void Particle::ClearForce()
{
	m_forceAccum.x = 0.0f;
	m_forceAccum.y = 0.0f;
	m_forceAccum.z = 0.0f;
}
