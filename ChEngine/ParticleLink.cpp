#include "ParticleLink.h"



ParticleLink::ParticleLink()
{
}


ParticleLink::~ParticleLink()
{
}

float ParticleLink::CurrentLength() const
{
	glm::vec3 RelativePos = m_Particles[0]->GetPosition() - m_Particles[1]->GetPosition();
	return glm::length(RelativePos);
}

ParticleCableLink::ParticleCableLink(Particle* particle_1, Particle* particle_2, float MaxLength, float Restitution)
	: m_MaxLength(MaxLength),
	  m_Restitution(Restitution)
{
	m_Particles[0] = particle_1;
	m_Particles[1] = particle_2;
}

int ParticleCableLink::AddContact(ParticleContact* contact, unsigned int limit) const
{
	// Find the length of the cable
	float length = CurrentLength();

	// Check if it's overextended
	if (length < m_MaxLength)
	{
		return 0;
	}

	// Otherwise, return the contact
	contact->m_Particles[0] = m_Particles[0];
	contact->m_Particles[1] = m_Particles[1];

	// normal
	glm::vec3 Normal = m_Particles[1]->GetPosition() - m_Particles[0]->GetPosition();
	contact->m_ContactNormal = glm::normalize(Normal);

	contact->m_Penetration = length - m_MaxLength;
	contact->m_Restitution = m_Restitution;
	contact->SetIsValid(true);

	return 1;
}
