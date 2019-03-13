#include "Contacts.h"
#include "Particle.h"


ParticleContact::ParticleContact()
{
	m_Particle_1 = nullptr;
	m_Particle_2 = nullptr;
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
	glm::vec3 RelativeVelocity = m_Particle_1->GetVelocity();
	if (m_Particle_2)
	{
		RelativeVelocity -= m_Particle_2->GetVelocity();
	}

	float SeparatingVelocity = glm::dot(RelativeVelocity, m_ContactNormal);
	return SeparatingVelocity;
}

void ParticleContact::ResolveVelocity(float duration)
{
	float SeparatingVelocity = CalculateSeparatingVelocity();

	if (SeparatingVelocity >= 0)
	{
		// the contact is either separating or stationary
		return;
	}

	// clamping restitution for small velocity
	float Restitution = abs(SeparatingVelocity) < 0.01f ? 0.0f : m_Restitution;

	// new separating velocity
	float NewSeparatingVelocity = -SeparatingVelocity * Restitution;

	// Check the velocity (in the direction of normal) build up due to acceleration only (for this frame)
	glm::vec3 RelativeAccel = m_Particle_1->GetAcceleration();
	RelativeAccel -= m_Particle_2 ? m_Particle_2->GetAcceleration() : glm::vec3(0.0f);
	float SeparatingVelocityBuildup = glm::dot(RelativeAccel, m_ContactNormal) * duration;

	// If we get a closing velocity due to acceleration buildup, remove it from the new separating velocity.
	if (SeparatingVelocityBuildup < 0)
	{
		NewSeparatingVelocity += Restitution * SeparatingVelocityBuildup;

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
	m_Particle_1->AddVelocity(ImpulsePerIMass * m_Particle_1->GetInverseMass());
	if (m_Particle_2)
	{
		// Particle 1 goes in the opposite direction
		m_Particle_2->AddVelocity(-ImpulsePerIMass * m_Particle_2->GetInverseMass());
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
	m_ParticleMovement_1 = MovePerIMass * m_Particle_1->GetInverseMass();
	m_Particle_1->AddPosition(m_ParticleMovement_1);
	if (m_Particle_2)
	{
		m_ParticleMovement_2 = -MovePerIMass * m_Particle_2->GetInverseMass();

	}
	if (m_Particle_2)
	{
		m_Particle_2->AddPosition(m_ParticleMovement_2);
	}
}

float ParticleContact::GetTotalInverseMass()
{
	float TotalInverseMass = m_Particle_1->GetInverseMass();
	TotalInverseMass += m_Particle_2 ? m_Particle_2->GetInverseMass() : 0;

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

void ParticleContactResolver::ResolveContacts(std::vector<std::shared_ptr<ParticleContact>>& Contacts, float duration)
{
	unsigned int i;
	m_IterationsUsed = 0;
	m_Iterations = Contacts.size();
	while (m_IterationsUsed < m_Iterations)
	{
		// find the contact with the largest closing velocity
		float max = FLT_MAX;
		unsigned int maxIndex = Contacts.size();
		for (i = 0; i < Contacts.size(); i++)
		{
			if(!Contacts[i]->IsValid()) continue;

			float SeparatingVel = Contacts[i]->CalculateSeparatingVelocity();
			if (SeparatingVel < max && (SeparatingVel < 0 || Contacts[i]->m_Penetration > 0))
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

		Contacts[maxIndex]->Resolve(duration);

		m_IterationsUsed++;
	}
}
