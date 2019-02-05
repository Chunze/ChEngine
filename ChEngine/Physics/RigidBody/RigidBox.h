#ifndef RIGID_BOX_H
#define RIGID_BOX_H

#include "RigidBody.h"

class RigidBox : public RigidBody
{
public:
	RigidBox();

	RigidBox(vec3 extend);

protected:
	vec3 m_Extend;

private:
	void ConstructInertiaTensor() override;
};

#endif // !BOX_COLLIDER_H
