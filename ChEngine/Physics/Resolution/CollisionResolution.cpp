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

		// Resolve interpenetration, which will update the position of same rigid body
		// in all other contacts

		collision.Resolve();
		m_PhysicsManager->Collisions.pop();
	}
}
