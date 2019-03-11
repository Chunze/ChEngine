#ifndef RIGID_BOX_H
#define RIGID_BOX_H

#include "RigidBody.h"

class RigidBox : public RigidBody
{
	typedef RigidBody Super;
public:
	RigidBox(PhysicsManager* Manager);

	RigidBox(PhysicsManager* Manager, vec3 extend);

protected:
	vec3 m_HalfSize;
	void ConstructInertiaTensor() override;
	void ConstructDefaultCollision() override;

private:
	
};

#endif // !BOX_COLLIDER_H
