#ifndef COLLISION_TEST_SELECTOR_H
#define COLLISION_TEST_SELECTOR_H

#include "CollisionTests.h"
#include "CollisionPrimitives.h"

struct CollisionTestSelector
{
	SphereVsSphere	SphereVsSphereTest;
	SphereVsBox		SphereVsBoxTest;

	CollisionTest* SelectCollisionTest(PhysicsPrimitive* Primitive1, PhysicsPrimitive* Primitive2)
	{
		if ((int)(Primitive1->GetType()) > (int)(Primitive2->GetType()))
		{
			return nullptr;
		}

		if (Primitive1->GetType() == PrimitiveType::SPHERE && Primitive2->GetType() == PrimitiveType::SPHERE)
		{
			return &SphereVsSphereTest;
		}
		else if (Primitive1->GetType() == PrimitiveType::SPHERE && Primitive2->GetType() == PrimitiveType::BOX)
		{
			return &SphereVsBoxTest;
		}
	}
};

#endif // !COLLISION_TEST_SELECTOR_H
