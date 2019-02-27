#pragma once
#include <memory>

#include "glm.h"
#include "CollisionPrimitives.h"

class PhysicsManager;

class RigidBody : std::enable_shared_from_this<RigidBody>
{
public:
	RigidBody();
	~RigidBody();

	float m_InverseMass = 1.0f;

	/// As long as the tensor is finite, it will be invertible.
	/// The inverse tensor is used for similar reasons to inverse mass
	mat3 m_InverseInertiaTensor;

	/// Damping apply to the linear motion
	float m_LinearDamping = -1.0f;
	float m_AngularDamping = -1.0f;

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

	void AddPosition(const vec3 &Displacement) { m_Position += Displacement; }

	void Integrate(float duration);

	void SetInertirTensor(const mat3 &IntertiaTensor);
	void CalculateDerivedData();

	/**    getters    **/
	mat4 GetTransform();
	vec3 GetPosition() { return m_Position; }
	CollisionPrimitive_sp GetCollisionPrimitive() { return m_CollisionPrimitive; }
	// gets the linear velocity of a point
	vec3 GetLinearVelocity(vec3 Point);

	/**    setters    **/
	void SetTransform(mat4 Transform);
	void SetPosition(vec3 Position) { m_Position = Position; }
	void SetVelocity(vec3 Velocity) { m_Velocity = Velocity; }
	void SetPhisicsManager(PhysicsManager* PhysicsManager) { m_PhysicsManager = PhysicsManager; }

	// Currently, I only allow 1 physics primitive.
	// Multiple primitive on 1 rigid body will be supported.
	void AddCollisionPrimitive(CollisionPrimitive_sp PrimitiveToAdd, const mat4 &Offset = mat4());

protected:
	PhysicsManager* m_PhysicsManager;

	bool m_bIsAwake;
	bool m_bUseGravity;
	vec3 m_ForceAccum;
	vec3 m_Acceleration;
	vec3 m_TorqueAccum;
	vec3 m_AngularAcceleration;

	// Collision shape that helps with the collision detection
	CollisionPrimitive_sp m_CollisionPrimitive;

	bool bDerivedDataReady = false;

	virtual void ConstructInertiaTensor() = 0;

	void ClearAccumulators();

};
