#ifndef COLLISION_TESTS_H
#define COLLISION_TESTS_H

#include "BodyContactData.h"
#include "glm.h"

class CollisionPrimitive;
class BoxPrimitive;
class SurfasePrimitive;
struct CollisionInfo;

struct CollisionTest
{
	virtual bool RunTest(CollisionPrimitive* Primitive1, CollisionPrimitive* Primitive2, CollisionInfo *Info) = 0;
};

struct SphereVsSphere : public CollisionTest
{
	bool RunTest(CollisionPrimitive* Primitive1, CollisionPrimitive* Primitive2, CollisionInfo *Info) override;
};

struct SphereVsBox : public CollisionTest
{
	bool RunTest(CollisionPrimitive* Primitive1, CollisionPrimitive* Primitive2, CollisionInfo *Info) override;
};

struct SphereVsSurface : public CollisionTest
{
	bool RunTest(CollisionPrimitive* Primitive1, CollisionPrimitive* Primitive2, CollisionInfo *Info) override;
};

struct BoxVsSurface : public CollisionTest
{
	bool RunTest(CollisionPrimitive* Primitive1, CollisionPrimitive* Primitive2, CollisionInfo *Info) override;
};

struct BoxVsBox : public CollisionTest
{
	bool RunTest(CollisionPrimitive* Primitive1, CollisionPrimitive* Primitive2, CollisionInfo *Info) override;
};

#endif