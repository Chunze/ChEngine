#include "BodyContactData.h"
#include "CollisionPrimitives.h"
#include "CollisionTestUtils.h"
#include "MathStatics.h"

bool CollisionTestUtils::BoxVsSurfaceEarlyOut(BoxPrimitive* Box, SurfasePrimitive* Surface)
{
	float RadiusSq = glm::dot(Box->m_HalfSize, Box->m_HalfSize);
	float Distance = glm::dot(Box->GetPosition(), Surface->m_Normal) - Surface->m_Offset;

	// the center is under the surface, or Distance <= Radius
	if (Distance <= 0 || Distance * Distance <= RadiusSq)
	{
		return true;
	}

	return false;
}

float CollisionTestUtils::TransformBoxToAxis(BoxPrimitive *Box, const vec3 &Axis)
{
	vec3 NormaliedAxis = glm::normalize(Axis);

	return Box->m_HalfSize.x * abs(glm::dot(NormaliedAxis, Box->GetAxis(0))) +
		Box->m_HalfSize.y * abs(glm::dot(NormaliedAxis, Box->GetAxis(1))) +
		Box->m_HalfSize.z * abs(glm::dot(NormaliedAxis, Box->GetAxis(2)));
}

float CollisionTestUtils::BoxPenetrationOnAxis(BoxPrimitive *Box1, BoxPrimitive *Box2, const vec3 &Axis)
{
	vec3 NormalizedAxis = glm::normalize(Axis);
	vec3 ToCenter = Box2->GetPosition() - Box1->GetPosition();
	float Distance = abs(glm::dot(ToCenter, NormalizedAxis));

	float ProjectOne = TransformBoxToAxis(Box1, Axis);
	float ProjectTwo = TransformBoxToAxis(Box2, Axis);

	return ProjectOne + ProjectTwo - Distance;
}

vec3 CollisionTestUtils::GetClosestPointOf2Lines(const vec3 &Axis1, const vec3 &Axis2, const vec3 &PointOnAxis1, const vec3 &PointOnAxis2)
{
	// vector between the test points on each edge
	vec3 Vec = PointOnAxis1 - PointOnAxis2;

	float VecDistance1 = glm::dot(Vec, Axis1);
	float VecDistance2 = glm::dot(Vec, Axis2);

	// Work out how far along each edge is the closest point
	// see: http://geomalgorithms.com/a07-_distance.html
	float a = Math::LengthSq(Axis1);
	float b = glm::dot(Axis1, Axis2);
	float c = Math::LengthSq(Axis2);
	float d = glm::dot(Vec, Axis1);
	float e = glm::dot(Vec, Axis2);
	float denom = a * c - b * b;

	float Move1 = (b * e - c * d) / denom;
	float Move2 = (a * e - b * d) / denom;

	vec3 CloestPt1 = PointOnAxis1 + Move1 * Axis1;
	vec3 CloestPt2 = PointOnAxis2 + Move2 * Axis2;

	return 0.5f * (CloestPt1 + CloestPt2);
}

void CollisionTestUtils::GetContactInfoFaceVsVertex(BoxPrimitive *Box1, BoxPrimitive *Box2, const vec3 &Axis, BodyContact &Contact)
{
	vec3 AxisLocal = Axis;
	vec3 ToCenter = Box2->GetPosition() - Box1->GetPosition();
	if (glm::dot(ToCenter, AxisLocal) > 0)
	{
		AxisLocal *= -1.0f;
	}

	// Find which vertex is in contact (in box2's coordinates)
	mat3 WorldToBox = mat3(Box2->GetInverseWorldTransform());
	vec3 Normal = WorldToBox * ToCenter;
	vec3 Vertex = Box2->m_HalfSize;
	if (glm::dot(Box2->GetAxis(0), ToCenter) > 0) Vertex.x = -Vertex.x;
	if (glm::dot(Box2->GetAxis(1), ToCenter) > 0) Vertex.y = -Vertex.y;
	if (glm::dot(Box2->GetAxis(2), ToCenter) > 0) Vertex.z = -Vertex.z;

	Vertex = Math::Multiply(Box2->GetWorldTransform(), Vertex);

	Contact.SetBodies(Box1->GetBody(), Box2->GetBody());
	Contact.SetContactNormal(AxisLocal);
	Contact.SetContactPoint(Vertex);
}

void CollisionTestUtils::GetContactInfoEdgeVsEdge(BoxPrimitive *Box1, BoxPrimitive *Box2, int AxisIndex1, int AxisIndex2, BodyContact &Contact)
{
	vec3 PointOnEdge1 = Box1->m_HalfSize;
	vec3 PointOnEdge2 = Box2->m_HalfSize;

	vec3 Axis = glm::cross(Box1->GetAxis(AxisIndex1), Box2->GetAxis(AxisIndex2));

	// Contact normal should point from box2 to box1
	vec3 ToCenter = Box1->GetPosition() - Box2->GetPosition();
	if (glm::dot(ToCenter, Axis) < 0)
	{
		Axis *= -1.0f;
	}

	// Snap the point to the edges (in local space)
	for (size_t i = 0; i < 3; i++)
	{
		if (i == AxisIndex1) PointOnEdge1[i] = 0; 
		else if (glm::dot(Box1->GetAxis(i), Axis) > 0) PointOnEdge1[i] = -PointOnEdge1[i];

		if (i == AxisIndex2) PointOnEdge2[i] = 0;
		else if (glm::dot(Box2->GetAxis(i), Axis) < 0) PointOnEdge2[i] = -PointOnEdge2[i];
	}

	

	// Convert to world space
	PointOnEdge1 = Math::Multiply(Box1->GetWorldTransform(), PointOnEdge1);
	PointOnEdge2 = Math::Multiply(Box2->GetWorldTransform(), PointOnEdge2);

	Contact.SetBodies(Box1->GetBody(), Box2->GetBody());
	Contact.SetContactPoint(
		GetClosestPointOf2Lines(Box1->GetAxis(AxisIndex1), Box2->GetAxis(AxisIndex2), PointOnEdge1, PointOnEdge2));
	Contact.SetContactNormal(Axis);
}