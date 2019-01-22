#pragma once
#include "glm.h"
#include <vector>

class Particle;

class ParticleContact
{
public:
	ParticleContact();
	~ParticleContact();


	/// Holds the particles that are involved in the contact.
	/// The second of these 2 can be nullptr for contacts with static object.
	Particle* m_Particles[2];

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

private:
	/// Handles the impulse calculations for this collision
	void ResolveVelocity(float duration);

	/// Handles the interpenetration resolution of this contact
	void ResolveInterpenetration(float duration);

	/// Returns the total inverse mass of the 2 particles
	float GetTotalInverseMass();
};

/// The contact resolution routine for particle contacts.
/// One instance can be shared for the entire simulation.
class ParticleContactResolver
{
public:
	ParticleContactResolver(unsigned int iterations);

	void SetIteration(unsigned int iterations);

	void ResolveContacts(std::vector<ParticleContact>& Contacts, float duration);

protected:
	/// Holds the number of iterations allowed
	unsigned int m_Iterations;

	/// The actual number of iterations used
	unsigned int m_IterationsUsed;
};

