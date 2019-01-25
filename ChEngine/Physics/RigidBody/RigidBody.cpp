#include "RigidBody.h"

static inline void CalculateTransformMat(mat4& OutMatrix, const vec3 &Position, const quat &Orientation)
{
	mat4 RotationMatrix = mat4_cast(Orientation);
	RotationMatrix[3] = glm::vec4(Position.x, Position.y, Position.z, 1.0f);
	OutMatrix = RotationMatrix;
}


RigidBody::RigidBody()
{
}


RigidBody::~RigidBody()
{
}

void RigidBody::CalculateDerivedData()
{
	CalculateTransformMat(m_Transform, m_Position, m_Orientation);
}
