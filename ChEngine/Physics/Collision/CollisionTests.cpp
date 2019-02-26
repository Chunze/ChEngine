#include <assert.h>

#include "CollisionTests.h"
#include "CollisionPrimitives.h"
#include "Contacts.h"

bool SphereVsSphere::RunTest(CollisionPrimitive* Primitive1, CollisionPrimitive* Primitive2, CollisionInfo& Info)
{
	assert(Primitive1->GetType() == PrimitiveType::SPHERE);
	assert(Primitive2->GetType() == PrimitiveType::SPHERE);

	SpherePrimitive* Sphere_1 = static_cast<SpherePrimitive*>(Primitive1);
	SpherePrimitive* Sphere_2 = static_cast<SpherePrimitive*>(Primitive2);

	vec3 Position_1 = Sphere_1->GetPosition();
	vec3 Position_2 = Sphere_2->GetPosition();

	vec3 DistanceVec = Position_2 - Position_1;
	float Distance = glm::length(DistanceVec);

	// Check distance vs radius sum
	if (Distance <= 0 || Distance >= Sphere_1->m_Radius + Sphere_2->m_Radius)
	{
		return false;
	}

	BodyContact Contact;

	Contact.SetBodies(Sphere_1->GetBody(), Sphere_2->GetBody());
	Contact.m_ContactNormal = DistanceVec / Distance;
	Contact.SetContactPoint(Position_2 + DistanceVec * 0.5f);
	Contact.m_Penetration = Sphere_1->m_Radius + Sphere_2->m_Radius - Distance;
	
	Info.AddContact(Contact);

	return true;
}

bool SphereVsBox::RunTest(CollisionPrimitive* Primitive1 /* Sphere */, CollisionPrimitive* Primitive2 /* Box */, CollisionInfo& Info)
{
	return false;
}

bool SphereVsSurface::RunTest(CollisionPrimitive* Primitive1 /* Sphere */, CollisionPrimitive* Primitive2 /* Surface */, CollisionInfo& Info)
{
	assert(Primitive1->GetType() == PrimitiveType::SPHERE);
	assert(Primitive2->GetType() == PrimitiveType::SURFACE);

	SpherePrimitive* Sphere = static_cast<SpherePrimitive*>(Primitive1);
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
	Contact.m_ContactNormal = -Surface->m_Normal;
	Contact.m_Penetration = -Distance;
	Contact.m_ContactPoint = Position - Surface->m_Normal * (Sphere->m_Radius - 0.5f * Distance);
	Info.AddContact(Contact);
	return true;
}

bool BoxVsSurface::RunTest(CollisionPrimitive* Primitive1 /* Box */, CollisionPrimitive* Primitive2 /* Surface */, CollisionInfo& Info)
{
	return false;
}
