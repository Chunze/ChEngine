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
	while (!m_CollisionDetection->m_PotentialContacts.empty())
	{
		CollisionPrimitive* Primitive1 = nullptr;
		CollisionPrimitive* Primitive2 = nullptr;
		auto potentialContact = m_CollisionDetection->m_PotentialContacts.front();
		m_CollisionDetection->m_PotentialContacts.pop();

		switch (potentialContact->ContactType)
		{
		case PotentialContactType::BODY_VS_BODY:
			Primitive1 = std::static_pointer_cast<PotentialBodyVsBodyContact>(potentialContact)->m_Body1->GetCollisionPrimitive().get();
			Primitive2 = std::static_pointer_cast<PotentialBodyVsBodyContact>(potentialContact)->m_Body2->GetCollisionPrimitive().get();
			break;
		case PotentialContactType::BODY_VS_PRIMITIVE:
			Primitive1 = std::static_pointer_cast<PotentialBodyVsPrimiveContact>(potentialContact)->m_Body->GetCollisionPrimitive().get();
			Primitive2 = std::static_pointer_cast<PotentialBodyVsPrimiveContact>(potentialContact)->m_Primitive.get();
		default:
			break;
		}

		if (!Primitive1 || !Primitive2)
		{
			continue;
		}

		if ((int)(Primitive1->GetType()) > (int)(Primitive2->GetType()))
		{
			CollisionPrimitive* Temp = Primitive2;
			Primitive2 = Primitive1;
			Primitive1 = Temp;
		}

		CollisionTest* Test = m_CollisionTestSelector.SelectCollisionTest(Primitive1, Primitive2);

		CollisionInfo Info;
		bool HasCollision = Test->RunTest(Primitive1, Primitive2, Info);
		if (HasCollision && m_CollisionDetection)
		{
			m_CollisionDetection->ReportCollision(Info);
		}
	}
}

