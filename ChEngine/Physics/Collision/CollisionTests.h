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

struct BoxVsBox : public CollisionTest
{
	bool RunTest(CollisionPrimitive* Primitive1, CollisionPrimitive* Primitive2, CollisionInfo& Info) override;
};

/// Contains fast tests whether two provided shapes are colliding each other
namespace IntersectionTests
{
	static bool BoxVsSurface(BoxPrimitive* Box, SurfasePrimitive* Surface);
}