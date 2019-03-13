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

enum class PotentialContactType
{
	BODY_VS_BODY,
	BODY_VS_PRIMITIVE
};

struct PotentialContact
{
	PotentialContactType ContactType;
};

/// This is the output of the broad phase of the collision detection
struct PotentialBodyVsBodyContact : public PotentialContact
{
	PotentialBodyVsBodyContact(RigidBody_sp Body1, RigidBody_sp Body2)
		: m_Body1(Body1), m_Body2(Body2)
	{
		ContactType = PotentialContactType::BODY_VS_BODY;
	}
	/// The two bodies that might be in contact
	RigidBody_sp m_Body1;
	RigidBody_sp m_Body2;
};

struct PotentialBodyVsPrimiveContact : public PotentialContact
{
	PotentialBodyVsPrimiveContact(RigidBody_sp Body, CollisionPrimitive_sp Primitive)
		: m_Body(Body), m_Primitive(Primitive)
	{
		ContactType = PotentialContactType::BODY_VS_PRIMITIVE;
	}

	RigidBody_sp m_Body;
	CollisionPrimitive_sp m_Primitive;
};


/// This is the output of the narrow phase of the collision detection
struct BodyContact
{
	RigidBody_sp m_RigidBody[2] = {nullptr, nullptr};

	// vectors points from origin of rigid body to contact point
	vec3 m_RelativeContactPosition[2];

	vec3 m_ContactPoint;

	vec3 m_ContactNormal;

	float m_Penetration;
	float m_TheRestitution;

	static const float m_Restitution;
	static const float m_VelocityLimit;
	static const float m_AngularLimit;
	static const float m_EnergyLoss;

	mat3 m_ContactToWorld;
	mat3 m_WorldToContact;

	/** intermediate values **/
	float m_AngularInertia[2];

	// This closing velocity is in contact space, so component X is the speed.
	vec3 m_ClosingVelocity;

	bool ConstructContactToWorld();
	void SetContactPoint(vec3 ContactPoint);
	void SetContactNormal(vec3 ContactNormal);
	void SetContactPenetration(float Penetration);
	void CalculateAngularInertia();
	void CalculateClosingVelocity();
	void CalculateClosingVelocity(vec3 DeltaClosingVelocity);

	void ResolveVelocity(vec3 LinearChange[2], vec3 AngularChange[2], float Delta);
	void ResolveInterpenetration(vec3 LinearChange[2], vec3 AngularChange[2]);
	float GetTotalInverseMass();

	void SetBodies(RigidBody_sp Body_1, RigidBody_sp Body_2)
	{
		// if bodies are already set, make sure that input bodies are the same ones
		if (m_RigidBody[0])
		{
			assert(m_RigidBody[0] == Body_1);
		}
		else
		{
			m_RigidBody[0] = Body_1;
		}

		if (m_RigidBody[1])
		{
			assert(m_RigidBody[1] == Body_2);
		}
		else
		{
			m_RigidBody[1] = Body_2;
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

