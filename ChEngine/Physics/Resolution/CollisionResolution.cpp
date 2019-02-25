#include "CollisionResolution.h"
#include "MathStatics.h"
#include "PhysicsManager.h"

CollisionResolution::CollisionResolution(PhysicsManager *physicsManager)
	: m_PhysicsManager(physicsManager)
{

}

void CollisionResolution::RunCollisionResolution()
{
	while (!m_PhysicsManager->Collisions.empty())
	{
		CollisionInfo collision = m_PhysicsManager->Collisions.front();
		m_PhysicsManager->Collisions.pop();
		if (collision.m_Contacts.size() == 0)
		{
			continue;
		}

		for (auto &Contact : collision.m_Contacts)
		{
			Contact.Resolve();
		}

	}
}
