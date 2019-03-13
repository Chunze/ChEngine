#include "CollisionInfo.h"

class CollisionPrimitive;
class BoxPrimitive;
class SurfasePrimitive;

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

/// Contains fast tests whether two provided shapes are colliding each other
namespace CollisionTestUtils
{
	static bool BoxVsSurfaceEarlyOut(BoxPrimitive* Box, SurfasePrimitive* Surface);

	static float TransformBoxToAxis(BoxPrimitive *Box, const vec3 &Axis);

	static float BoxPenetrationOnAxis(BoxPrimitive *Box1, BoxPrimitive *Box2, const vec3 &Axis);

	// Returns mid point of the 2 lines
	static vec3 GetClosestPointOf2Lines(const vec3 &Axis1, const vec3 &Axis2, const vec3 &PointOnAxis1, const vec3 &PointOnAxis2);

	// populate contact for box vs box, face vs vertex situation (does not populate penetration)
	static void GetContactInfoFaceVsVertex(BoxPrimitive *Box1, BoxPrimitive *Box2, const vec3 &Axis, BodyContact &Contact);

	static void GetContactInfoEdgeVsEdge(BoxPrimitive *Box1, BoxPrimitive *Box2, int AsixIndex1, int AsixIndex2, BodyContact &Contact);
}