#include "ParticleLink.h"



ParticleLink::ParticleLink()
{
}


ParticleLink::~ParticleLink()
{
}

float ParticleLink::CurrentLength() const
{
	glm::vec3 RelativePos = m_Particle_1->GetPosition() - m_Particle_2->GetPosition();
	return glm::length(RelativePos);
}

ParticleCableLink::ParticleCableLink(std::shared_ptr<Particle> particle_1, std::shared_ptr<Particle> particle_2, float MaxLength, float Restitution)
	: m_MaxLength(MaxLength),
	  m_Restitution(Restitution)
{
	m_Particle_1 = particle_1;
	m_Particle_2 = particle_2;
}

int ParticleCableLink::AddContact(std::shared_ptr<ParticleContact> contact, unsigned int limit) const
{
	// Find the length of the cable
	float length = CurrentLength();

	// Check if it's overextended
	if (length < m_MaxLength)
	{
		return 0;
	}

	// Otherwise, return the contact
	contact->m_Particle_1 = m_Particle_1;
	contact->m_Particle_2 = m_Particle_2;

	// normal
	glm::vec3 Normal = m_Particle_2->GetPosition() - m_Particle_1->GetPosition();
	contact->m_ContactNormal = glm::normalize(Normal);

	contact->m_Penetration = length - m_MaxLength;
	contact->m_Restitution = m_Restitution;
	contact->SetIsValid(true);

	return 1;
}
