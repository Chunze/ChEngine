#include "RigidBody.h"
#include "PhysicsManager.h"
#include "MathStatics.h"

static inline void CalculateTransformMat(mat4& OutMatrix, const vec3 &Position, const quat &Orientation)
{
	mat4 RotationMatrix = mat4_cast(Orientation);
	RotationMatrix[3] = glm::vec4(Position.x, Position.y, Position.z, 1.0f);
	OutMatrix = RotationMatrix;
}

static inline void CalculateInertiaTensor(mat3& iitWorld, const mat3 &iitBody, const mat3 &rotMatrix)
{
	mat3 InverseRotMatrix = glm::inverse(rotMatrix);
	iitWorld = rotMatrix * iitBody * InverseRotMatrix;
}


RigidBody::RigidBody()
{
	m_InverseMass = 1.0f;
}


RigidBody::~RigidBody()
{
}

void RigidBody::AddForce(const vec3 &force)
{
	m_ForceAccum += force;
	m_bIsAwake = true;
}

void RigidBody::AddForceAtPoint(const vec3 &force, const vec3 &point)
{
	// direction relative to center of mass
	vec3 Direction = point - m_Position;

	m_ForceAccum += force;
	m_TorqueAccum += glm::cross(Direction, force);

	m_bIsAwake = true;
}

void RigidBody::AddForceAtBodyPoint(const vec3 &force, const vec3 &point)
{
	// Convert the point into world space
	vec3 WorldPoint = m_Transform * vec4(point, 1.0f);
	AddForceAtPoint(force, WorldPoint);
}

void RigidBody::Integrate(float duration)
{
	// calculate linear acceleration
	m_Acceleration = m_ForceAccum * m_InverseMass;

	// calculate angular acceleration
	m_AngularAcceleration = m_InverseTensorWorld * m_TorqueAccum;

	// adjust velocity
	m_Velocity += m_Acceleration * duration;
	m_AngularVelocity += m_AngularAcceleration * duration;

	// apply damping
	if (m_LinearDamping <= 0.0f)
	{
		m_LinearDamping = m_PhysicsManager->GetLinearDamping();
	}

	if (m_AngularDamping <= 0.0f)
	{
		m_AngularDamping = m_PhysicsManager->GetAngularDamping();
	}

	m_Velocity *= m_LinearDamping;
	m_AngularVelocity *= m_AngularDamping;

	// adjust position and rotation
	m_Position += m_Velocity * duration;
	m_Orientation = Math::Rotate(m_Orientation, m_AngularVelocity, duration);

	// update derived data
	CalculateDerivedData();

	// clear accumulators
	ClearAccumulators();
}

void RigidBody::SetInertirTensor(const mat3 &IntertiaTensor)
{
	m_InverseInertiaTensor = glm::inverse(IntertiaTensor);
}

void RigidBody::CalculateDerivedData()
{
	// calculate the transform matrix for the body
	CalculateTransformMat(m_Transform, m_Position, m_Orientation);

	// calculate the inverse inertia tensor in world space
	mat3 RotationMatrix = mat3(m_Transform);
	CalculateInertiaTensor(m_InverseTensorWorld, m_InverseInertiaTensor, RotationMatrix);
}

void RigidBody::ClearAccumulators()
{
	m_ForceAccum = vec3(0.0f);
	m_TorqueAccum = vec3(0.0f);
}
