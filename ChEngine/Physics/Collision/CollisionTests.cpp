#include <assert.h>

#include "CollisionInfo.h"
#include "CollisionTests.h"
#include "CollisionTestUtils.h"
#include "CollisionPrimitives.h"
#include "Contacts.h"
#include "MathStatics.h"

static float mults[8][3] = { { 1.0f,  1.0f, 1.0f}, {-1.0f, 1.0f,  1.0f}, {1.0f, -1.0f,  1.0f}, { 1.0f,  1.0f, -1.0f},
							 {-1.0f, -1.0f, 1.0f}, {-1.0f, 1.0f, -1.0f}, {1.0f, -1.0f, -1.0f}, {-1.0f, -1.0f, -1.0f} };

bool SphereVsSphere::RunTest(CollisionPrimitive* Primitive1, CollisionPrimitive* Primitive2, CollisionInfo *Info)
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

	BodyContact *Contact = Info->GetNewContact();

	Contact->SetBodies(Sphere_1->GetBody(), Sphere_2->GetBody());
	Contact->SetContactNormal(DistanceVec);
	Contact->SetContactPoint(Position_2 + DistanceVec * 0.5f);
	Contact->SetContactPenetration(Sphere_1->m_Radius + Sphere_2->m_Radius - Distance);

	return true;
}

bool SphereVsBox::RunTest(CollisionPrimitive* Primitive1 /* Sphere */, CollisionPrimitive* Primitive2 /* Box */, CollisionInfo *Info)
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

	BodyContact *Contact = Info->GetNewContact();
	Contact->SetBodies(Sphere->GetBody(), Box->GetBody());
	Contact->SetContactNormal(Center - ClosestPtWorld);
	Contact->SetContactPenetration(Sphere->m_Radius - glm::sqrt(Distance));
	Contact->SetContactPoint(ClosestPtWorld);

	return true;
}

bool SphereVsSurface::RunTest(CollisionPrimitive* Primitive1 /* Sphere */, CollisionPrimitive* Primitive2 /* Surface */, CollisionInfo *Info)
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
	BodyContact *Contact = Info->GetNewContact();
	Contact->SetBodies(Sphere->GetBody(), Surface->GetBody());
	Contact->SetContactNormal(Surface->m_Normal);
	Contact->SetContactPenetration(-Distance);
	Contact->SetContactPoint(Position - Surface->m_Normal * (Sphere->m_Radius - 0.5f * Distance));

	return true;
}

bool BoxVsSurface::RunTest(CollisionPrimitive* Primitive1 /* Box */, CollisionPrimitive* Primitive2 /* Surface */, CollisionInfo *Info)
{
	assert(Primitive1->GetType() == PrimitiveType::BOX);
	assert(Primitive2->GetType() == PrimitiveType::SURFACE);

	BoxPrimitive*	  Box     = static_cast<BoxPrimitive*>(Primitive1);
	SurfasePrimitive* Surface = static_cast<SurfasePrimitive*>(Primitive2);

	bool bHasCollision = false;

	if (!CollisionTestUtils::BoxVsSurfaceEarlyOut(Box, Surface))
	{
		return bHasCollision;
	}

	// we have a collision, check each vertex
	// If the box is resting on a plane or an edge, it will report 4 or 2 contact points.

	for (unsigned int i = 0; i < 8; i++)
	{
		// get vertex in world space
		vec3 VertexPosition = vec3(mults[i][0], mults[i][1], mults[i][2]) * Box->m_HalfSize;
		VertexPosition = vec3(Box->GetWorldTransform() * vec4(VertexPosition, 1.0f));

		// calculate the distance from the plane
		float VertexDistance = glm::dot(VertexPosition, Surface->m_Normal);

		// compare to plane's distance
		if (VertexDistance < Surface->m_Offset)
		{
			// Collision!
			BodyContact *Contact = Info->GetNewContact();
			Contact->SetBodies(Box->GetBody(), Surface->GetBody());
			Contact->SetContactNormal(Surface->m_Normal);
			Contact->SetContactPenetration(Surface->m_Offset - VertexDistance);
			Contact->SetContactPoint(VertexPosition + 0.5f * Surface->m_Normal * (Surface->m_Offset - VertexDistance));
			bHasCollision = true;
		}
	}

	return bHasCollision;
}

bool BoxVsBox::RunTest(CollisionPrimitive* Primitive1, CollisionPrimitive* Primitive2, CollisionInfo *Info)
{
	assert(Primitive1->GetType() == PrimitiveType::BOX);
	assert(Primitive2->GetType() == PrimitiveType::BOX);

	BoxPrimitive* Box1 = static_cast<BoxPrimitive*>(Primitive1);
	BoxPrimitive* Box2 = static_cast<BoxPrimitive*>(Primitive2);

	// 15 separating axis need to be checked for Box vs Box
	vec3 axis[15];
	// Face axis
	axis[0] = Box1->GetAxis(0); axis[1] = Box1->GetAxis(1); axis[2] = Box1->GetAxis(2);
	axis[3] = Box2->GetAxis(0); axis[4] = Box2->GetAxis(1); axis[5] = Box2->GetAxis(2);

	// edge-edge axis
	axis[6] = glm::cross(axis[0], axis[3]);
	axis[7] = glm::cross(axis[0], axis[4]);
	axis[8] = glm::cross(axis[0], axis[5]);
	axis[9] = glm::cross(axis[1], axis[3]);
	axis[10] = glm::cross(axis[1], axis[4]);
	axis[11] = glm::cross(axis[1], axis[5]);
	axis[12] = glm::cross(axis[2], axis[3]);
	axis[13] = glm::cross(axis[2], axis[4]);
	axis[14] = glm::cross(axis[2], axis[5]);

	// SAT
	float BestOverlap = FLT_MAX;
	size_t BestCase;
	for (size_t index = 0; index < 15; index++)
	{
		vec3 Axis = axis[index];

		// Check for axes that were generated by (almost) parallel vectors;
		if (Math::LengthSq(Axis) < SMALL_NUMBER) continue;
		Axis = glm::normalize(Axis);

		float Overlap = CollisionTestUtils::BoxPenetrationOnAxis(Box1, Box2, Axis);
		if (Overlap <= 0)
		{
			return false;
		}
		if (Overlap < BestOverlap)
		{
			BestOverlap = Overlap;
			BestCase = index;
		}
	}

	BodyContact Contact;
	if(BestCase < 3)
	{
		// box1's face vs box2's vertex
		CollisionTestUtils::GetContactInfoFaceVsVertex(Box1, Box2, axis[BestCase], Contact);
		Contact.m_Penetration = BestOverlap;

		// populate contact features
		Contact.m_ContactFeature.m_FeatureType = ContactFeatureType::FACE_VS_VERTEX;
		Contact.m_ContactFeature.m_Primitives[0] = Box1;
		Contact.m_ContactFeature.m_Primitives[1] = Box2;
		Contact.m_ContactFeature.m_FeatureID[0] = BestCase;
	}
	else if(BestCase < 6)
	{
		// box2's face vs box1's vertex
		CollisionTestUtils::GetContactInfoFaceVsVertex(Box2, Box1, axis[BestCase], Contact);
		Contact.m_Penetration = BestOverlap;

		// populate contact features
		Contact.m_ContactFeature.m_FeatureType = ContactFeatureType::FACE_VS_VERTEX;
		Contact.m_ContactFeature.m_Primitives[0] = Box2;
		Contact.m_ContactFeature.m_Primitives[1] = Box1;
		Contact.m_ContactFeature.m_FeatureID[0] = BestCase - 3;
	}
	else
	{
		// edge vs edge
		// Find the index of the axis that was used to construct the SAT axis
		int AxisIndex1, AxisIndex2;
		if (BestCase == 6 || BestCase == 7 || BestCase == 8) AxisIndex1 = 0;
		else if (BestCase == 9 || BestCase == 10 || BestCase == 11) AxisIndex1 = 1;
		else if (BestCase == 12 || BestCase == 13 || BestCase == 14) AxisIndex1 = 2;

		if (BestCase == 6 || BestCase == 9 || BestCase == 12) AxisIndex2 = 0;
		else if (BestCase == 7 || BestCase == 10 || BestCase == 13) AxisIndex2 = 1;
		else if (BestCase == 8 || BestCase == 11 || BestCase == 14) AxisIndex2 = 2;


		Contact.SetContactPenetration(BestOverlap);
		CollisionTestUtils::GetContactInfoEdgeVsEdge(Box1, Box2, AxisIndex1, AxisIndex2, Contact);

		// populate contact features
		Contact.m_ContactFeature.m_FeatureID[0] = AxisIndex1;
		Contact.m_ContactFeature.m_FeatureID[1] = AxisIndex2;
		Contact.m_ContactFeature.m_FeatureType = ContactFeatureType::EDGE_VS_EDGE;
		Contact.m_ContactFeature.m_Primitives[0] = Box1;
		Contact.m_ContactFeature.m_Primitives[1] = Box2;
	}

	Info->AddContact(Contact);


	return true;
}
