#pragma once
#include "glm.h"


class RigidBody
{
public:
	RigidBody();
	~RigidBody();

	float m_InverseMass;

	/// As long as the tensor is finite, it will be invertible.
	/// The inverse tensor is used for similar reasons to inverse mass
	mat3 m_InverseInertiaTensor;

	/// Damping apply to the linear motion
	float m_LinearDamping;

	vec3 m_Position;
	quat m_Orientation;

	/// Linear velocity of the body
	vec3 m_Velocity;

	/// Angular velocity of the body
	vec3 m_AngularVelocity;

	/***    Derived data    ***/
	/// The transform matrix
	mat4 m_Transform;

	mat3 m_InverseTensorWorld;

	/// Adds the given force to center of mass of the rigid body
	/// Forces are given in world space
	void AddForce(const vec3 &force);

	/// Adds the given force to the given point on the rigid body.
	/// Both the force and the application point are given in the world
	/// space. Because the force is not applied a the center of mass,
	/// it may be split into both a force and torque
	void AddForceAtPoint(const vec3 &force, const vec3 &point);

	/// Adds the given force to the given point on the rigid body.
	/// The force is in world space, and the point is in body space.
	void AddForceAtBodyPoint(const vec3 &force, const vec3 &point);

	void Integrate(float duration);

	void SetInertirTensor(const mat3 &IntertiaTensor);
	void CalculateDerivedData();

protected:
	bool m_bIsAwake;
	vec3 m_ForceAccum;
	vec3 m_TorqueAccum;

	void ClearAccumulators();
};
