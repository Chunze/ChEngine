#ifndef COLLISION_TEST_SELECTOR_H
#define COLLISION_TEST_SELECTOR_H

#include "CollisionTests.h"
#include "CollisionPrimitives.h"

struct CollisionTestSelector
{
	SphereVsSphere	SphereVsSphereTest;
	SphereVsBox		SphereVsBoxTest;
	SphereVsSurface SphereVsSurfaceTest;
	BoxVsSurface	BoxVsSurfaceTest;
	BoxVsBox		BoxVsBoxTest;

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
		else if (Primitive1->GetType() == PrimitiveType::BOX && Primitive2->GetType() == PrimitiveType::SURFACE)
		{
			return &BoxVsSurfaceTest;
		}
		else if (Primitive1->GetType() == PrimitiveType::BOX && Primitive2->GetType() == PrimitiveType::BOX)
		{
			return &BoxVsBoxTest;
		}

		return nullptr;
	}
};

#endif // !COLLISION_TEST_SELECTOR_H
