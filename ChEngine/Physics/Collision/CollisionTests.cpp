#include <assert.h>

#include "CollisionTests.h"
#include "CollisionPrimitives.h"
#include "Contacts.h"
#include "MathStatics.h"

bool SphereVsSphere::RunTest(CollisionPrimitive* Primitive1, CollisionPrimitive* Primitive2, CollisionInfo& Info)
{
	assert(Primitive1->GetType() == PrimitiveType::SPHERE);
	assert(Primitive2->GetType() == PrimitiveType::SPHERE);

	SpherePrimitive* Sphere_1 = static_cast<SpherePrimitive*>(Primitive1);
	SpherePrimitive* Sphere_2 = static_cast<SpherePrimitive*>(Primitive2);

	vec3 Position_1 = Sphere_1->GetPosition();
	vec3 Position_2 = Sphere_2->GetPosition();

	vec3 DistanceVec = Position_1 - Position_2;
	float Distance = glm::length(DistanceVec);

	// Check distance vs radius sum
	if (Distance <= 0 || Distance >= Sphere_1->m_Radius + Sphere_2->m_Radius)
	{
		return false;
	}

	BodyContact Contact;

	Contact.SetBodies(Sphere_1->GetBody(), Sphere_2->GetBody());
	Contact.SetContactNormal(DistanceVec);
	Contact.SetContactPoint(Position_2 + DistanceVec * 0.5f);
	Contact.SetContactPenetration(Sphere_1->m_Radius + Sphere_2->m_Radius - Distance);
	
	Info.AddContact(Contact);

	return true;
}

bool SphereVsBox::RunTest(CollisionPrimitive* Primitive1 /* Sphere */, CollisionPrimitive* Primitive2 /* Box */, CollisionInfo& Info)
{
	assert(Primitive1->GetType() == PrimitiveType::SPHERE);
	assert(Primitive2->GetType() == PrimitiveType::BOX);

	SpherePrimitive*  Sphere = static_cast<SpherePrimitive*>(Primitive1);
	BoxPrimitive*	  Box    = static_cast<BoxPrimitive*>(Primitive2);

	// convert sphere center into box space
	vec3 Center = Sphere->GetPosition();
	mat4 WorldToBox = glm::inverse(Box->GetWorldTransform());
	vec3 RelCenter = WorldToBox * vec4(Center, 1.0);

	// Early out
	if (glm::abs(RelCenter.x) - Sphere->m_Radius > Box->m_HalfSize.x ||
		glm::abs(RelCenter.y) - Sphere->m_Radius > Box->m_HalfSize.y ||
		glm::abs(RelCenter.z) - Sphere->m_Radius > Box->m_HalfSize.z)
	{
		return false;
	}

	// find the closest point on the box to the sphere
	vec3 ClosestPoint(0.0f);
	float Distance;

	// clamp each coordinate to the box
	Distance = RelCenter.x;
	if (Distance > Box->m_HalfSize.x) Distance = Box->m_HalfSize.x;
	if (Distance < -Box->m_HalfSize.x) Distance = -Box->m_HalfSize.x;
	ClosestPoint.x = Distance;

	Distance = RelCenter.y;
	if (Distance > Box->m_HalfSize.y) Distance = Box->m_HalfSize.y;
	if (Distance < -Box->m_HalfSize.y) Distance = -Box->m_HalfSize.y;
	ClosestPoint.y = Distance;

	Distance = RelCenter.z;
	if (Distance > Box->m_HalfSize.z) Distance = Box->m_HalfSize.z;
	if (Distance < -Box->m_HalfSize.z) Distance = -Box->m_HalfSize.z;
	ClosestPoint.z = Distance;

	// check to see if in contact
	Distance = Math::LengthSq(ClosestPoint - RelCenter);
	if (Distance > Sphere->m_Radius * Sphere->m_Radius)
	{
		return false;
	}

	// collision!
	vec3 ClosestPtWorld = Box->GetWorldTransform() * vec4(ClosestPoint, 1.0f);

	BodyContact Contact;
	Contact.SetBodies(Sphere->GetBody(), Box->GetBody());
	Contact.SetContactNormal(Center - ClosestPtWorld);
	Contact.SetContactPenetration(Sphere->m_Radius - glm::sqrt(Distance));
	Contact.SetContactPoint(ClosestPtWorld);
	Info.AddContact(Contact);

	return true;
}

bool SphereVsSurface::RunTest(CollisionPrimitive* Primitive1 /* Sphere */, CollisionPrimitive* Primitive2 /* Surface */, CollisionInfo& Info)
{
	assert(Primitive1->GetType() == PrimitiveType::SPHERE);
	assert(Primitive2->GetType() == PrimitiveType::SURFACE);

	SpherePrimitive*  Sphere  = static_cast<SpherePrimitive*>(Primitive1);
	SurfasePrimitive* Surface = static_cast<SurfasePrimitive*>(Primitive2);

	vec3 Position = Sphere->GetPosition();

	// find the distance from the plane
	float Distance = glm::dot(Surface->m_Normal, Position) - Sphere->m_Radius - Surface->m_Offset;

	if (Distance >= 0)
	{
		return false;
	}

	// Collision!
	BodyContact Contact;
	Contact.SetBodies(Sphere->GetBody(), Surface->GetBody());
	Contact.SetContactNormal(Surface->m_Normal);
	Contact.SetContactPenetration(-Distance);
	Contact.SetContactPoint(Position - Surface->m_Normal * (Sphere->m_Radius - 0.5f * Distance));
	Info.AddContact(Contact);
	return true;
}

bool BoxVsSurface::RunTest(CollisionPrimitive* Primitive1 /* Box */, CollisionPrimitive* Primitive2 /* Surface */, CollisionInfo& Info)
{
	assert(Primitive1->GetType() == PrimitiveType::BOX);
	assert(Primitive2->GetType() == PrimitiveType::SURFACE);

	BoxPrimitive*	  Box     = static_cast<BoxPrimitive*>(Primitive1);
	SurfasePrimitive* Surface = static_cast<SurfasePrimitive*>(Primitive2);

	if (!IntersectionTests::BoxVsSurface(Box, Surface))
	{
		return false;
	}

	// we have a collision, check each vertex
	// If the box is resting on a plane or an edge, it will report 4 or 2 contact points.
	static float mults[8][3] = { { 1.0f,  1.0f, 1.0f}, {-1.0f, 1.0f,  1.0f}, {1.0f, -1.0f,  1.0f}, { 1.0f,  1.0f, -1.0f},
								 {-1.0f, -1.0f, 1.0f}, {-1.0f, 1.0f, -1.0f}, {1.0f, -1.0f, -1.0f}, {-1.0f, -1.0f, -1.0f} };

	for (unsigned int i = 0; i < 8; i++)
	{
		// get vertex in world space
		vec3 VertexPosition = vec3(mults[i][0], mults[i][1], mults[i][2]) * Box->m_HalfSize;
		VertexPosition = vec3(Box->GetWorldTransform() * vec4(VertexPosition, 1.0f));

		// calculate the distance from the plane
		float VertexDistance = glm::dot(VertexPosition, Surface->m_Normal);

		// compare to plane's distance
		if (VertexDistance <= Surface->m_Offset)
		{
			// Collision!
			BodyContact Contact;
			Contact.SetBodies(Box->GetBody(), Surface->GetBody());
			Contact.SetContactNormal(Surface->m_Normal);
			Contact.SetContactPenetration(Surface->m_Offset - VertexDistance);
			Contact.SetContactPoint(VertexPosition + 0.5f * Surface->m_Normal * (Surface->m_Offset - VertexDistance));
			Info.AddContact(Contact);
		}
	}

	return true;
}

bool IntersectionTests::BoxVsSurface(BoxPrimitive* Box, SurfasePrimitive* Surface)
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

bool BoxVsBox::RunTest(CollisionPrimitive* Primitive1, CollisionPrimitive* Primitive2, CollisionInfo& Info)
{
	return false;
}
