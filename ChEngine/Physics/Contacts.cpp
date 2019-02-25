#include "Contacts.h"
#include "MathStatics.h"
#include "Particle.h"
#include "RigidBody.h"


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

bool BodyContact::ConstructContactToWorld()
{
	// TODO: assume there is only one contact point for now
	vec3 x = m_ContactNormal;
	vec3 y(0.0f, 1.0f, 0.0f);
	vec3 z;
	// To make the calculation stable, choose X-axis as the second basis if x is near Y-axis
	if (glm::abs(x.x) <= glm::abs(x.y))
	{
		y.x = 1.0f;
		y.y = 0.0f;
	}

	bool bSuccessful = Math::MakeOrthonormalBasis(x, y, z);

	if (bSuccessful)
	{
		m_ContactToWorld = mat3(x, y, z);
		m_WorldToContact = glm::transpose(m_ContactToWorld);
	}

	return bSuccessful;
}

void BodyContact::SetContactPoint(vec3 ContactPoint)
{
	m_ContactPoint = ContactPoint;
	m_RelativeContactPosition1 = m_ContactPoint - m_RigidBody1->GetPosition();
	if (m_RigidBody2)
	{
		m_RelativeContactPosition1 = m_ContactPoint - m_RigidBody2->GetPosition();
	}
}

void BodyContact::ResolveInterpenetration()
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
	vec3 RigidBodyDisplacement = MovePerIMass * m_RigidBody1->m_InverseMass;
	m_RigidBody1->AddPosition(RigidBodyDisplacement);
	if (m_RigidBody2)
	{
		RigidBodyDisplacement = -MovePerIMass * m_RigidBody2->m_InverseMass;
		m_RigidBody2->AddPosition(RigidBodyDisplacement);
	}
}

float BodyContact::GetTotalInverseMass()
{
	float TotalInverseMass = m_RigidBody1->m_InverseMass;
	TotalInverseMass += m_RigidBody2 ? m_RigidBody2->m_InverseMass : 0;

	return TotalInverseMass;
}
