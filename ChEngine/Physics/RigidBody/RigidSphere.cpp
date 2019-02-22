#include "RigidSphere.h"

RigidSphere::RigidSphere(float Radius)
	: m_Radius(Radius)
{
	ConstructInertiaTensor();
}

RigidSphere::RigidSphere()
{
	// Default 0.5 radius
	m_Radius = 0.5f;
	ConstructInertiaTensor();
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

