#include "CollisionDetection.h"

CollisionDetection::CollisionDetection()
	: m_BroadPhaseTest(this),
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

