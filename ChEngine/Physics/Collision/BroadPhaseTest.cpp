#include "BroadPhaseTest.h"
#include "PhysicsManager.h"

BroadPhaseTest::BroadPhaseTest(CollisionDetection* collisionDetection)
{
	m_CollisionDetection = collisionDetection;
}

void BroadPhaseTest::RunTest()
{
	RigidBodies rigid_bodies = *m_CollisionDetection->m_PhysicsManager->GetRigidBodies();
	CollisionPrimitives primitives = *m_CollisionDetection->GetRegisteredPrimitives();
	for (auto x = rigid_bodies.begin(); x != rigid_bodies.end(); ++x)
	{
		// against other rigid bodies
		for (auto y = x + 1; y != rigid_bodies.end(); ++y)
		{
			//PotentialBodyVsBodyContact potentialContact(*x, *y);
			m_CollisionDetection->m_PotentialContacts.push(std::make_shared<PotentialBodyVsBodyContact>(*x, *y));
		}

		// against other primitives 
		for (auto p = primitives.begin(); p != primitives.end(); ++p)
		{
			//PotentialBodyVsPrimiveContact potentialContact(*x, *p);
			m_CollisionDetection->m_PotentialContacts.push(std::make_shared<PotentialBodyVsPrimiveContact>(*x, *p));
		}
	}
}

