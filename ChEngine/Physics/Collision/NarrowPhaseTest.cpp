#include "CollisionDetection.h"
#include "CollisionPrimitives.h"
#include "Contacts.h"
#include "NarrowPhaseTest.h"
#include "PhysicsTypes.h"
#include "RigidBody.h"

NarrowPhaseTest::NarrowPhaseTest(CollisionDetection* collisionDetection)
{
	m_CollisionDetection = collisionDetection;
}

void NarrowPhaseTest::RunTest()
{
	for (auto potentialContact : m_CollisionDetection->m_PotentialContacts)
	{
		CollisionPrimitive* Primitive1 = potentialContact->m_Body1->GetCollisionPrimitive().get();
		CollisionPrimitive* Primitive2 = potentialContact->m_Body2->GetCollisionPrimitive().get();

		CollisionTest* Test = m_CollisionTestSelector.SelectCollisionTest(Primitive1, Primitive2);

		CollisionInfo Info;
		Test->RunTest(Primitive1, Primitive2, Info);
	}
}

