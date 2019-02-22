#ifndef RIGID_SPHERE_H
#define RIGID_SPHERE_H

#include "RigidBody.h"

class RigidSphere : public RigidBody
{
public:
	RigidSphere();

	RigidSphere(float Radius);

protected:
	float m_Radius;

private:
	void ConstructInertiaTensor() override;
};

#endif // !RIGID_SPHERE_H
