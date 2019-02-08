#include "BroadPhaseTest.h"
#include "PhysicsManager.h"

BroadPhaseTest::BroadPhaseTest(CollisionDetection* collisionDetection)
{
	m_CollisionDetection = collisionDetection;
}

void BroadPhaseTest::RunTest()
{
	RigidBodies rigid_bodies = *m_CollisionDetection->m_PhysicsManager->GetRigidBodies();
	for (auto x = rigid_bodies.begin(); x != rigid_bodies.end(); ++x)
	{
		for (auto y = x + 1; y != rigid_bodies.end(); ++y)
		{
			PotentialBodyContact_sp potentialContact = make_shared<PotentialBodyContact>(*x, *y);
			m_CollisionDetection->m_PotentialContacts.push_back(potentialContact);
		}
	}
}

