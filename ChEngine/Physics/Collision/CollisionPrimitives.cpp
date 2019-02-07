#include "CollisionPrimitives.h"
#include "RigidBody.h"
vec3 CollisionPrimitive::GetPosition()
{
	return m_RigidBody->GetPosition();
}

