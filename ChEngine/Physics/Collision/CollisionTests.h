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
namespace IntersectionTestsUtils
{
	static bool BoxVsSurfaceEarlyOut(BoxPrimitive* Box, SurfasePrimitive* Surface);

	static float TransformBoxToAxis(BoxPrimitive *Box, const vec3 &Axis);

	static float BoxPenetrationOnAxis(BoxPrimitive *Box1, BoxPrimitive *Box2, const vec3 &Axis);

	// Returns mid point of the 2 lines
	static vec3 GetClosestPointOf2Lines(const vec3 &Axis1, const vec3 &Axis2, const vec3 &PointOnAxis1, const vec3 &PointOnAxis2);
}