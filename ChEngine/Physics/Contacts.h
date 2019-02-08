#pragma once
#include "glm.h"
#include "PhysicsTypes.h"
#include <vector>

class Particle;

class ParticleContact
{
public:
	ParticleContact();
	~ParticleContact();


	/// Holds the particles that are involved in the contact.
	/// The second of these 2 can be nullptr for contacts with static object.
	Particle_sp m_Particle_1 = nullptr;
	Particle_sp m_Particle_2 = nullptr;

	glm::vec3 m_ParticleMovement_1;
	glm::vec3 m_ParticleMovement_2;

	/// Restitution coefficient
	float m_Restitution;

	/// Contact normal in world space
	glm::vec3 m_ContactNormal;

	/// Holds the depth of penetration at the contact
	float m_Penetration;

	/// Resolves this contact for both velocity and interpenetration
	void Resolve(float duration);

	/// Calculates the separating velocity at this contact
	float CalculateSeparatingVelocity() const;

	void SetIsValid(bool IsValid) { bIsValid = IsValid; }
	bool IsValid() { return bIsValid; }

protected:
	bool bIsValid = false;

private:
	/// Handles the impulse calculations for this collision
	void ResolveVelocity(float duration);

	/// Handles the interpenetration resolution of this contact
	void ResolveInterpenetration(float duration);

	/// Returns the total inverse mass of the 2 particles
	float GetTotalInverseMass();
};

/// This is the output of the broad phase of the collision detection
struct PotentialBodyContact
{
	PotentialBodyContact(RigidBody_sp Body1, RigidBody_sp Body2)
		: m_Body1(Body1), m_Body2(Body2)
	{}
	/// The two bodies that might be in contact
	RigidBody_sp m_Body1;
	RigidBody_sp m_Body2;
};


/// This is the output of the narrow phase of the collision detection
struct BodyContact
{
	RigidBody_sp m_RigidBody1;
	RigidBody_sp m_RigidBody2;

	vec3 m_ContactPoint;

	vec3 m_ContactNormal;

	float m_Penetration;

	void SetBodies(RigidBody_sp Body_1, RigidBody_sp Body_2)
	{
		// if bodies are already set, make sure that input bodies are the same ones
		if (m_RigidBody1)
		{
			assert(m_RigidBody1 == Body_1);
		}
		else
		{
			m_RigidBody1 = Body_1;
		}

		if (m_RigidBody2)
		{
			assert(m_RigidBody2 == Body_2);
		}
		else
		{
			m_RigidBody2 = Body_2;
		}
	}

	bool operator==(const BodyContact &OtherContact)
	{
		return m_ContactPoint == OtherContact.m_ContactPoint;
	}

	bool operator!=(const BodyContact &OtherContact)
	{
		return !(m_ContactPoint == OtherContact.m_ContactPoint);
	}
};

/// The contact resolution routine for particle contacts.
/// One instance can be shared for the entire simulation.
class ParticleContactResolver
{
public:
	ParticleContactResolver(unsigned int iterations);

	void SetIteration(unsigned int iterations);

	void ResolveContacts(std::vector<std::shared_ptr<ParticleContact>>& Contacts, float duration);

protected:
	/// Holds the number of iterations allowed
	unsigned int m_Iterations = 0;

	/// The actual number of iterations used
	unsigned int m_IterationsUsed;
};

