#ifndef BOX_COLLIDER_H
#define BOX_COLLIDER_H

#include "RigidBody.h"

class BoxCollider : public RigidBody
{
public:
	BoxCollider();

	BoxCollider(vec3 extend);

protected:
	vec3 m_Extend;

private:
	void ConstructInertiaTensor() override;
};

#endif // !BOX_COLLIDER_H
