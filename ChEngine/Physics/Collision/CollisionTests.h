#include "CollisionInfo.h"

class CollisionPrimitives;

struct CollisionTest
{
	virtual bool RunTest(CollisionPrimitives* Primitive1, CollisionPrimitives* Primitive2, CollisionInfo& Info) = 0;
};

struct SphereVsSphere : public CollisionTest
{
	bool RunTest(CollisionPrimitives* Primitive1, CollisionPrimitives* Primitive2, CollisionInfo& Info) override;
};

struct SphereVsBox : public CollisionTest
{
	bool RunTest(CollisionPrimitives* Primitive1, CollisionPrimitives* Primitive2, CollisionInfo& Info) override;
};