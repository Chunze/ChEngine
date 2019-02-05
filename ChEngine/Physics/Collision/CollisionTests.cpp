#include "CollisionTests.h"

bool SphereVsSphere::RunTest(CollisionPrimitives* Primitive1, CollisionPrimitives* Primitive2, CollisionInfo& Info)
{
	return false;
}

bool SphereVsBox::RunTest(CollisionPrimitives* Primitive1, CollisionPrimitives* Primitive2, CollisionInfo& Info)
{
	return false;
}
