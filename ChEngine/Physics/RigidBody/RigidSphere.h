#ifndef RIGID_SPHERE_H
#define RIGID_SPHERE_H

#include "RigidBody.h"

class RigidSphere : public RigidBody
{
	typedef RigidBody Super;
public:
	RigidSphere(PhysicsManager* Manager);

	RigidSphere(PhysicsManager* Manager, float Radius);

protected:
	float m_Radius;
	void ConstructDefaultCollision() override;
	void ConstructInertiaTensor() override;

private:
};

#endif // !RIGID_SPHERE_H
