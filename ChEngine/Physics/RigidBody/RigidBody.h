#pragma once
#include "glm.h"


class RigidBody
{
public:
	RigidBody();
	~RigidBody();

	float m_InverseMass;

	/// Damping apply to the linear motion
	float m_LinearDamping;

	vec3 m_Position;
	quat m_Orientation;

	/// Linear velocity of the body
	vec3 m_Velocity;

	/// Angular velocity of the body
	vec3 m_AngularVelocity;

	/// The transform matrix
	mat4 m_Transform;

	void CalculateDerivedData();
};
