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
	if (m_PhysicsManager->Collisions.size() > 0)
	{
		SetMaxIteration(m_PhysicsManager->Collisions.size(), ResolutionIterationMultiplier);

		PrepareContacts();

		ResolvePenetrations();

		ResolveVelocities(duration);
	}

	m_PhysicsManager->Collisions.clear();
}

void CollisionResolution::PrepareContacts()
{
	// ContactInfo may contain more than 1 actual contacts.
	for (auto &collision : m_PhysicsManager->Collisions)
	{
		collision.CombineContacts();
		if (collision.m_IsValid)
		{
			collision.m_FinalContact.ConstructContactToWorld();
			collision.m_FinalContact.CalculateClosingVelocity();
		}
	}
}

void CollisionResolution::ResolvePenetrations()
{
	auto Collisions = &m_PhysicsManager->Collisions;

	size_t MovedIndex;
	float MaxPenetration;
	unsigned int IterationUsed = 0;

	vec3 LinearChange[2];
	vec3 AngularChange[2];

	while (IterationUsed <= m_MaxIteration)
	{
		// find contact with largest penetration
		MaxPenetration = 0.0f;
		MovedIndex = m_NumberContacts;

		for (size_t i = 0; i < m_NumberContacts; i++)
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
		if (MovedIndex == m_NumberContacts)
		{
			break;
		}

		// TODO: set Awake status for bodies here.


		// Resolving penetration for this contact
		(*Collisions)[MovedIndex].m_FinalContact.ResolveInterpenetration(/* Out */LinearChange, /* Out */AngularChange);

		// This action may have changed the penetration of other contacts contain the same body,
		// so update the contacts.
		for (size_t i = 0; i < m_NumberContacts; i++)
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

void CollisionResolution::ResolveVelocities(float duration)
{
	auto Collisions = &m_PhysicsManager->Collisions;

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
			if ((*Collisions)[i].m_IsValid)
			{
				if ((*Collisions)[i].m_FinalContact.m_ClosingVelocity.x < MaxClosingVelocity)
				{
					MaxClosingVelocity = (*Collisions)[i].m_FinalContact.m_ClosingVelocity.x;
					MovedIndex = i;
				}
			}
		}

		// There is no contact that has negative closing velocity
		if (MovedIndex == m_NumberContacts)
		{
			break;
		}

		// TODO: set Awake status for bodies here.


		// Resolving velocities for this contact
		(*Collisions)[MovedIndex].m_FinalContact.ResolveVelocity(/* Out */LinearChange, /* Out */AngularChange, duration);

		// recalculate the closing velocities of the contacts that have the save bodies
		for (size_t i = 0; i < m_NumberContacts; i++)
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
							float sign = MatchedIndex ? -1.0f : 1.0f;
							vec3 VelocityChange = LinearChange[MovedBodyIndex] +
								glm::cross(AngularChange[MovedBodyIndex], (*Collisions)[i].m_FinalContact.m_RelativeContactPosition[MatchedIndex]);
							(*Collisions)[i].m_FinalContact.CalculateClosingVelocity(sign * VelocityChange);
						}
					}
				}
			}
		}

		IterationUsed++;
	}
}

