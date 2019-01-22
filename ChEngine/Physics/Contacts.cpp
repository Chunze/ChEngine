#include "Contacts.h"
#include "Particle.h"


ParticleContact::ParticleContact()
{
	m_Particles[0] = nullptr;
	m_Particles[1] = nullptr;
}


ParticleContact::~ParticleContact()
{
}

void ParticleContact::Resolve(float duration)
{
	ResolveVelocity(duration);
	ResolveInterpenetration(duration);
}

float ParticleContact::CalculateSeparatingVelocity() const
{
	glm::vec3 RelativeVelocity = m_Particles[0]->GetVelocity();
	if (m_Particles[1])
	{
		RelativeVelocity -= m_Particles[1]->GetVelocity();
	}

	float SeparatingVelocity = glm::dot(RelativeVelocity, m_ContactNormal);
	return SeparatingVelocity;
}

void ParticleContact::ResolveVelocity(float duration)
{
	float SeparatingVelocity = CalculateSeparatingVelocity();

	if (SeparatingVelocity > 0)
	{
		// the contact is either separating or stationary
		return;
	}

	// new separating velocity
	float NewSeparatingVelocity = -SeparatingVelocity * m_Restitution;

	// Check the velocity (in the direction of normal) build up due to acceleration only (for this frame)
	glm::vec3 RelativeAccel = m_Particles[0]->GetAcceleration();
	RelativeAccel -= m_Particles[1] ? m_Particles[1]->GetAcceleration() : glm::vec3(0.0f);
	float SeparatingVelocityBuildup = glm::dot(RelativeAccel, m_ContactNormal) * duration;

	// If we get a closing velocity due to acceleration buildup, remove it from the new separating velocity.
	if (SeparatingVelocityBuildup < 0)
	{
		NewSeparatingVelocity += m_Restitution * SeparatingVelocityBuildup;

		// Make sure we haven't removed more than there was to remove
		if (NewSeparatingVelocity < 0)
		{
			NewSeparatingVelocity = 0;
		}
	}

	float DeltaVelocity = NewSeparatingVelocity - SeparatingVelocity;

	// Apply the change in velocity to each object in proportion to their
	// inverse mass
	float TotalInverseMass = GetTotalInverseMass();

	if (TotalInverseMass <= 0.0f)
	{
		return;
	}
	
	// Impulse to apply
	float impulse = DeltaVelocity / TotalInverseMass;

	// Find the amount of impulse per unit of inverse mass
	glm::vec3 ImpulsePerIMass = m_ContactNormal * impulse;

	// Apply impulse: they are applied in the direction of the contact,
	// and are proportional to the inverse mass
	m_Particles[0]->AddVelocity(ImpulsePerIMass * m_Particles[0]->GetInverseMass());
	if (m_Particles[1])
	{
		// Particle 1 goes in the opposite direction
		m_Particles[1]->AddVelocity(-ImpulsePerIMass * m_Particles[1]->GetInverseMass());
	}
}

void ParticleContact::ResolveInterpenetration(float duration)
{
	if (m_Penetration <= 0)
	{
		// There isn't any penetration
		return;
	}

	// The movement of each object is backed on their inverse mass
	float TotalInverseMass = GetTotalInverseMass();

	// Find the amount of penetration resolution per unit of inverse mass
	glm::vec3 MovePerIMass = m_ContactNormal * (m_Penetration / TotalInverseMass);

	// Apply displacement
	m_Particles[0]->AddPosition(MovePerIMass * m_Particles[0]->GetInverseMass());
	if (m_Particles[1])
	{
		m_Particles[1]->AddPosition(-MovePerIMass * m_Particles[1]->GetInverseMass());
	}
}

float ParticleContact::GetTotalInverseMass()
{
	float TotalInverseMass = m_Particles[0]->GetInverseMass();
	TotalInverseMass += m_Particles[1] ? m_Particles[1]->GetInverseMass() : 0;

	return TotalInverseMass;
}

ParticleContactResolver::ParticleContactResolver(unsigned int iterations)
	: m_Iterations(iterations)
{

}

void ParticleContactResolver::SetIteration(unsigned int iterations)
{
	m_Iterations = iterations;
}

void ParticleContactResolver::ResolveContacts(std::vector<ParticleContact>& Contacts, float duration)
{
	unsigned int i;
	m_IterationsUsed = 0;
	while (m_IterationsUsed < m_Iterations)
	{
		// find the contact with the largest closing velocity
		float max = FLT_MAX;
		unsigned int maxIndex = Contacts.size();
		for (i = 0; i < Contacts.size(); i++)
		{
			float SeparatingVel = Contacts[i].CalculateSeparatingVelocity();
			if (SeparatingVel < max && (SeparatingVel < 0) || Contacts[i].m_Penetration > 0)
			{
				max = SeparatingVel;
				maxIndex = i;
			}
		}

		// Do we have anything worth resolving
		if (maxIndex == Contacts.size())
		{
			break;
		}

		Contacts[maxIndex].Resolve(duration);

		m_IterationsUsed++;
	}
}
