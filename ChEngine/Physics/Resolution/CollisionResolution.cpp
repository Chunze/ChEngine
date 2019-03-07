#include "CollisionResolution.h"
#include "MathStatics.h"
#include "PhysicsManager.h"

static const int ResolutionIterationMultiplier = 4;

CollisionResolution::CollisionResolution(PhysicsManager *physicsManager)
	: m_PhysicsManager(physicsManager)
{

}

void CollisionResolution::RunCollisionResolution()
{
	if (m_PhysicsManager->Collisions.size() > 0)
	{
		ResolvePenetrations();

		ResolveVelocities();
	}

	m_PhysicsManager->Collisions.clear();
}

void CollisionResolution::ResolvePenetrations()
{
	auto Collisions = &m_PhysicsManager->Collisions;
	size_t NumContacts = m_PhysicsManager->Collisions.size();

	for (size_t i = 0; i < NumContacts; i++)
	{
		(*Collisions)[i].CombineContacts();
	}

	unsigned int MovedIndex;
	float MaxPenetration;

	int MaxIteration = NumContacts * ResolutionIterationMultiplier;
	int IterationUsed = 0;
	while (IterationUsed <= MaxIteration)
	{
		// find contact with largest penetration
		MaxPenetration = 0.0f;
		MovedIndex = NumContacts;
		for (size_t i = 0; i < NumContacts; i++)
		{
			if ((*Collisions)[i].m_IsValid)
			{
				if ((*Collisions)[i].m_FinalContact.m_Penetration > MaxPenetration)
				{
					MaxPenetration = (*Collisions)[i].m_FinalContact.m_Penetration;
					MovedIndex = i;
				}
			}
		}

		// There are no contact that has positive penetration
		if (MovedIndex == NumContacts)
		{
			break;
		}

		// TODO: set Awake status for bodies here.

		vec3 LinearChange[2];
		vec3 AngularChange[2];

		// Resolving penetration for this contact
		(*Collisions)[MovedIndex].ResolveInterPenetration(/* Out */LinearChange, /* Out */AngularChange);

		// This action may have changed the penetration of other contacts contain the same body,
		// so update the contacts.
		for (size_t i = 0; i < NumContacts; i++)
		{
			// check each body in the contact
			for (unsigned MatchedIndex = 0; MatchedIndex < 2; MatchedIndex++)
			{
				if ((*Collisions)[i].m_FinalContact.m_RigidBody[MatchedIndex])
				{
					// Check for a match with each body in the newly resolved contact
					for (unsigned MovedBodyIndex = 0; MovedBodyIndex < 2; MovedBodyIndex++)
					{
						if ((*Collisions)[i].m_FinalContact.m_RigidBody[MatchedIndex] ==
							(*Collisions)[MovedIndex].m_FinalContact.m_RigidBody[MovedBodyIndex])
						{
							float sign = MatchedIndex ? 1.0f : -1.0f;
							vec3 PositionChange =
								LinearChange[MovedBodyIndex] + 
								glm::cross(AngularChange[MovedBodyIndex], (*Collisions)[i].m_FinalContact.m_RelativeContactPosition[MatchedIndex]);
							float Displacement = glm::dot(PositionChange, (*Collisions)[i].m_FinalContact.m_ContactNormal);
							(*Collisions)[i].m_FinalContact.m_Penetration += sign * Displacement;
						}
					}
				}
			}
		}

		IterationUsed++;
	}
}

void CollisionResolution::ResolveVelocities()
{
	for (auto Collision : m_PhysicsManager->Collisions)
	{
		Collision.ResolveVelocity();
	}
}
