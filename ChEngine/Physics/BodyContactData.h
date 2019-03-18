#pragma once
#include "glm.h"
#include "PhysicsTypes.h"

enum class PotentialContactType
{
	BODY_VS_BODY,
	BODY_VS_PRIMITIVE
};

struct PotentialContact
{
	PotentialContactType ContactType;
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

enum class ContactFeatureType
{
	NONE,
	FACE_VS_VERTEX,
	VERTEX_VS_FACE,
	EDGE_VS_EDGE
};

struct ContactFeature
{
	CollisionPrimitive* m_Primitives[2];
	unsigned int m_FramesLived = 0;
	ContactFeatureType m_FeatureType = ContactFeatureType::NONE;
	int m_FeatureID[2] = { -1, -1 };

	inline bool IsValid()
	{
		return m_FramesLived > 0 && m_FramesLived < 5;
	}

	bool operator==(const ContactFeature &Other) const
	{
		return m_Primitives[0] == Other.m_Primitives[0] &&
			m_Primitives[1] == Other.m_Primitives[1] &&
			m_FeatureID[0] == Other.m_FeatureID[0] &&
			m_FeatureID[1] == Other.m_FeatureID[1];
	}
};

struct ContactFeatureList
{
	ContactFeatures m_ContactFeatures;

	void UpdateFeatureList(ContactFeature FeatureToAdd)
	{
		// check if the contact feature exist
		for (ContactFeature &Feature : m_ContactFeatures)
		{
			if (Feature == FeatureToAdd)
			{
				Feature.m_FramesLived = 0;
				return;
			}
		}

		// out of the loop, did not find a match, add the feature to the list
		m_ContactFeatures.push_back(FeatureToAdd);
	}

	void CleanupFeatureList()
	{
		auto it = m_ContactFeatures.begin();
		while (it != m_ContactFeatures.end())
		{
			if (!it->IsValid())
			{
				it = m_ContactFeatures.erase(it);
			}
			else
			{
				it++;
			}
		}
	}
};

/// This is the output of the narrow phase of the collision detection
struct BodyContact
{
	RigidBody_sp m_RigidBody[2];

	// vectors points from origin of rigid body to contact point
	vec3 m_RelativeContactPosition[2];

	ContactFeature m_ContactFeature;

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