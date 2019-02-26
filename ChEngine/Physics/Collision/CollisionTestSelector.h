#ifndef COLLISION_TEST_SELECTOR_H
#define COLLISION_TEST_SELECTOR_H

#include "CollisionTests.h"
#include "CollisionPrimitives.h"

struct CollisionTestSelector
{
	SphereVsSphere	SphereVsSphereTest;
	SphereVsBox		SphereVsBoxTest;
	SphereVsSurface SphereVsSurfaceTest;

	CollisionTest* SelectCollisionTest(CollisionPrimitive* Primitive1, CollisionPrimitive* Primitive2)
	{
		if (!Primitive1 || !Primitive2)
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
		else if (Primitive1->GetType() == PrimitiveType::SPHERE && Primitive2->GetType() == PrimitiveType::SURFACE)
		{
			return &SphereVsSurfaceTest;
		}

		return nullptr;
	}
};

#endif // !COLLISION_TEST_SELECTOR_H
