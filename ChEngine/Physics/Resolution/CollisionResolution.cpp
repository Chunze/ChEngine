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
		collision.Resolve();
		m_PhysicsManager->Collisions.pop();
	}
}
