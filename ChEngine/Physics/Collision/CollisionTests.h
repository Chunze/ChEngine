#include "CollisionInfo.h"

class CollisionPrimitive;

struct CollisionTest
{
	virtual bool RunTest(CollisionPrimitive* Primitive1, CollisionPrimitive* Primitive2, CollisionInfo& Info) = 0;
};

struct SphereVsSphere : public CollisionTest
{
	bool RunTest(CollisionPrimitive* Primitive1, CollisionPrimitive* Primitive2, CollisionInfo& Info) override;
};

struct SphereVsBox : public CollisionTest
{
	bool RunTest(CollisionPrimitive* Primitive1, CollisionPrimitive* Primitive2, CollisionInfo& Info) override;
};

struct SphereVsSurface : public CollisionTest
{
	bool RunTest(CollisionPrimitive* Primitive1, CollisionPrimitive* Primitive2, CollisionInfo& Info) override;
};

struct BoxVsSurface : public CollisionTest
{
	bool RunTest(CollisionPrimitive* Primitive1, CollisionPrimitive* Primitive2, CollisionInfo& Info) override;
};