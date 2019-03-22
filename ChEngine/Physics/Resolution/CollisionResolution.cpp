#include "CollisionResolution.h"
#include "MathStatics.h"
#include "PhysicsManager.h"

static const int ResolutionIterationMultiplier = 4;

CollisionResolution::CollisionResolution(PhysicsManager *physicsManager)
	: m_PhysicsManager(physicsManager)
{

}

void CollisionResolution::SetMaxIteration(size_t ContactNum, unsigned Multiplier)
{
	m_NumberContacts = ContactNum;
	m_MaxIteration = m_NumberContacts * Multiplier;
}

void CollisionResolution::RunCollisionResolution(float duration)
{
	if (m_PhysicsManager->m_Collisions.HasCollision())
	{
		SetMaxIteration(m_PhysicsManager->m_Collisions.ContactCount(), ResolutionIterationMultiplier);

		PrepareContacts();

		ResolvePenetrations();

		ResolveVelocities(duration);
		
		m_PhysicsManager->m_Collisions.clearContacts();
	}

}

void CollisionResolution::PrepareContacts()
{
	// ContactInfo may contain more than 1 actual contacts.
	for (auto &collision : m_PhysicsManager->m_Collisions.m_Contacts)
	{
		collision.ConstructContactToWorld();
		collision.CalculateClosingVelocity();
	}
}

void CollisionResolution::ResolvePenetrations()
{
	auto Collisions = &m_PhysicsManager->m_Collisions.m_Contacts;

	size_t MovedIndex;
	float MaxPenetration;
	unsigned int IterationUsed = 0;

	vec3 LinearChange[2];
	vec3 AngularChange[2];

	while (IterationUsed <= m_MaxIteration)
	{
		IterationUsed++;

		// find contact with largest penetration
		MaxPenetration = SMALL_NUMBER;
		MovedIndex = m_NumberContacts;

		for (size_t i = 0; i < m_NumberContacts; i++)
		{
			if ((*Collisions)[i].m_Penetration > MaxPenetration)
			{
				MaxPenetration = (*Collisions)[i].m_Penetration;
				MovedIndex = i;
			}
		}

		// There are no contact that has positive penetration
		if (MovedIndex == m_NumberContacts)
		{
			break;
		}

		// TODO: set Awake status for bodies here.


		// Resolving penetration for this contact
		LinearChange[0] = vec3(0.0f);
		LinearChange[1] = vec3(0.0f);
		AngularChange[0] = vec3(0.0f);
		AngularChange[1] = vec3(0.0f);
		(*Collisions)[MovedIndex].ResolveInterpenetration(/* Out */LinearChange, /* Out */AngularChange);

		// This action may have changed the penetration of other contacts contain the same body,
		// so update the contacts.
		for (size_t i = 0; i < m_NumberContacts; i++)
		{
			// check each body in the contact
			for (unsigned MatchedIndex = 0; MatchedIndex < 2; MatchedIndex++)
			{
				if ((*Collisions)[i].m_RigidBody[MatchedIndex])
				{
					// Check for a match with each body in the newly resolved contact
					for (unsigned MovedBodyIndex = 0; MovedBodyIndex < 2; MovedBodyIndex++)
					{
						if ((*Collisions)[i].m_RigidBody[MatchedIndex] ==
							(*Collisions)[MovedIndex].m_RigidBody[MovedBodyIndex])
						{
							float sign = MatchedIndex ? 1.0f : -1.0f;
							vec3 PositionChange =
								LinearChange[MovedBodyIndex] + 
								glm::cross(AngularChange[MovedBodyIndex], (*Collisions)[i].m_RelativeContactPosition[MatchedIndex]);
							float Displacement = glm::dot(PositionChange, (*Collisions)[i].m_ContactNormal);
							(*Collisions)[i].m_Penetration += sign * Displacement;
						}
					}
				}
			}
		}
	}
}

void CollisionResolution::ResolveVelocities(float duration)
{
	auto Collisions = &m_PhysicsManager->m_Collisions.m_Contacts;

	size_t MovedIndex;
	float MaxClosingVelocity;
	size_t IterationUsed = 0;

	vec3 LinearChange[2];
	vec3 AngularChange[2];

	while (IterationUsed <= m_MaxIteration)
	{
		// find contact with largest closing velocity (closing velocity should be negative)
		MaxClosingVelocity = 0.0f;
		MovedIndex = m_NumberContacts;

		for (size_t i = 0; i < m_NumberContacts; i++)
		{
			if ((*Collisions)[i].m_ClosingVelocity.x < MaxClosingVelocity)
			{
				MaxClosingVelocity = (*Collisions)[i].m_ClosingVelocity.x;
				MovedIndex = i;
			}
		}

		// There is no contact that has negative closing velocity
		if (MovedIndex == m_NumberContacts)
		{
			break;
		}

		// TODO: set Awake status for bodies here.


		// Resolving velocities for this contact
		LinearChange[0] = vec3(0.0f);
		LinearChange[1] = vec3(0.0f);
		AngularChange[0] = vec3(0.0f);
		AngularChange[1] = vec3(0.0f);
		(*Collisions)[MovedIndex].ResolveVelocity(/* Out */LinearChange, /* Out */AngularChange, duration);

		// recalculate the closing velocities of the contacts that have the save bodies
		for (size_t i = 0; i < m_NumberContacts; i++)
		{
			// check each body in the contact
			for (unsigned MatchedIndex = 0; MatchedIndex < 2; MatchedIndex++)
			{
				if ((*Collisions)[i].m_RigidBody[MatchedIndex])
				{
					// Check for a match with each body in the newly resolved contact
					for (unsigned MovedBodyIndex = 0; MovedBodyIndex < 2; MovedBodyIndex++)
					{
						if ((*Collisions)[i].m_RigidBody[MatchedIndex] ==
							(*Collisions)[MovedIndex].m_RigidBody[MovedBodyIndex])
						{
							float sign = MatchedIndex ? -1.0f : 1.0f;
							vec3 VelocityChange = LinearChange[MovedBodyIndex] +
								glm::cross(AngularChange[MovedBodyIndex], (*Collisions)[i].m_RelativeContactPosition[MatchedIndex]);
							(*Collisions)[i].CalculateClosingVelocity(sign * VelocityChange);
						}
					}
				}
			}
		}

		IterationUsed++;
	}
}

