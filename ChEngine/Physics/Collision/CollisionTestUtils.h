#include "glm.h"

class SurfasePrimitive;
class BoxPrimitive;
struct BodyContact;

/// Contains fast tests whether two provided shapes are colliding each other
namespace CollisionTestUtils
{
	bool BoxVsSurfaceEarlyOut(BoxPrimitive* Box, SurfasePrimitive* Surface);

	float TransformBoxToAxis(BoxPrimitive *Box, const vec3 &Axis);

	float BoxPenetrationOnAxis(BoxPrimitive *Box1, BoxPrimitive *Box2, const vec3 &Axis);

	// Returns mid point of the 2 lines
	vec3 GetClosestPointOf2Lines(const vec3 &Axis1, const vec3 &Axis2, const vec3 &PointOnAxis1, const vec3 &PointOnAxis2);

	// populate contact for box vs box, face vs vertex situation (does not populate penetration)
	void GetContactInfoFaceVsVertex(BoxPrimitive *Box1, BoxPrimitive *Box2, const vec3 &Axis, BodyContact &Contact);

	void GetContactInfoEdgeVsEdge(BoxPrimitive *Box1, BoxPrimitive *Box2, int AsixIndex1, int AsixIndex2, BodyContact &Contact);
}