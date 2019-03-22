#include "CollisionPrimitives.h"
#include "RigidBody.h"

void CollisionPrimitive::SetBody(RigidBody_sp Body, const mat4 &Offset /*= mat4()*/)
{
	m_RigidBody = Body;
	m_OffsetTransform = Offset;
}

vec3 CollisionPrimitive::GetPosition()
{
	return vec3(m_OffsetTransform * vec4(m_RigidBody->GetPosition(), 1.0f));
}

mat4 CollisionPrimitive::GetWorldTransform()
{
	return m_RigidBody->GetTransform() * m_OffsetTransform;
}

mat4 CollisionPrimitive::GetInverseWorldTransform()
{
	return glm::inverse(GetWorldTransform());
}

vec3 CollisionPrimitive::GetAxis(int index)
{
	if (index < 0 || index > 3)
	{
		return vec3();
	}

	return glm::normalize(vec3(GetWorldTransform()[index]));
}

