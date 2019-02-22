#include "CollisionDetection.h"
#include "PhysicsManager.h"

CollisionDetection::CollisionDetection(PhysicsManager* PhysicsManager)
	: m_PhysicsManager(PhysicsManager),
	  m_BroadPhaseTest(this),
	  m_NarrowPhaseTest(this)
{
	
}

void CollisionDetection::RunCollisionDetection()
{
	m_BroadPhaseTest.RunTest();

	m_NarrowPhaseTest.RunTest();
}

void CollisionDetection::RegisterCollisionPrimitive(CollisionPrimitive_sp PrimitiveToAdd)
{
	// m_CollisionPrimitives.push_back(PrimitiveToAdd);
}

void CollisionDetection::RunBroadPhase()
{

}

void CollisionDetection::RunNarrowPhase()
{

}

