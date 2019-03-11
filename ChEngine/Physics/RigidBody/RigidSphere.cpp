#include "CollisionPrimitives.h"
#include "RigidSphere.h"

RigidSphere::RigidSphere(PhysicsManager* Manager, float Radius)
	: Super(Manager),
	m_Radius(Radius)
{
	ConstructInertiaTensor();
	ConstructDefaultCollision();
}

RigidSphere::RigidSphere(PhysicsManager* Manager)
	: Super(Manager)
{
	// Default 0.5 radius
	m_Radius = 0.5f;
	ConstructInertiaTensor();
	ConstructDefaultCollision();
}

void RigidSphere::ConstructDefaultCollision()
{
	m_CollisionPrimitive = std::make_shared<SpherePrimitive>(m_Radius);
}

void RigidSphere::ConstructInertiaTensor()
{
	mat3 InertiaTensor(0.0f);

	float RadiusSq = 0.4f * m_Radius * m_Radius;

	InertiaTensor[0][0] = RadiusSq;
	InertiaTensor[1][1] = RadiusSq;
	InertiaTensor[2][2] = RadiusSq;

	SetInertirTensor(InertiaTensor);
}

