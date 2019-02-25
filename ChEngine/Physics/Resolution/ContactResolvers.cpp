#include "Contacts.h"
#include "RigidBody.h"


const float BodyContact::m_Restitution = 0.f;

void BodyContact::Resolve(/*float duration*/)
{
	ResolveInterpenetration();

	// early out
	// Closing velocity in the contact space
	vec3 ContactVelocity = m_WorldToContact * m_RigidBody1->GetLinearVelocity(m_ContactPoint);
	if (m_RigidBody2)
	{
		ContactVelocity -= m_WorldToContact * m_RigidBody2->GetLinearVelocity(m_ContactPoint);
	}

	float DeltaVelocity = -ContactVelocity.x * (1 + m_Restitution);
// 	if (DeltaVelocity < 0.0f)
// 	{
// 		return;
// 	}

	/** Step 1: construct the contact coordinate axis **/

	ConstructContactToWorld();

	/** Step 2: calculate change in velocity of the contact point on each object per unit of impulse. **/

	// Linear velocity change caused by angular velocity change per unit of impulse
	vec3 Torque = glm::cross(m_RelativeContactPosition1, m_ContactNormal);
	vec3 AngularVelChange = m_RigidBody1->m_InverseInertiaTensor * Torque;
	vec3 LinearVelChange = glm::cross(AngularVelChange, m_RelativeContactPosition1);
	// Change in velocity in contact space
	float DeltaVelocity_PerUnitImpulse = glm::dot(LinearVelChange, m_ContactNormal);
	// Add the linear component of velocity change
	DeltaVelocity_PerUnitImpulse += m_RigidBody1->m_InverseMass;

	/** Closing velocity is calculated above **/

	/** Step 4: calculate the impulse needed to result the change in step 3 **/

	vec3 ContactImpulse, WorldImpulse;
	ContactImpulse.x = DeltaVelocity / DeltaVelocity_PerUnitImpulse;
	WorldImpulse = m_ContactToWorld * ContactImpulse;

	/** step 5: split the impulse into linear and angular components **/

	// Linear velocity change
	vec3 VelocityChange = WorldImpulse * m_RigidBody1->m_InverseMass;
	// Angular velocity change
	vec3 ImpulsiveTorque = glm::cross(WorldImpulse, m_ContactPoint - m_RigidBody1->m_Position);
	vec3 RotationChange = m_RigidBody1->m_InverseTensorWorld * ImpulsiveTorque;

	m_RigidBody1->m_Velocity += VelocityChange;
	m_RigidBody1->m_AngularVelocity += RotationChange;

	
	if (m_RigidBody2)
	{
		/** Step 2 **/
		Torque = glm::cross(m_RelativeContactPosition2, m_ContactNormal);
		AngularVelChange = m_RigidBody2->m_InverseInertiaTensor * Torque;
		LinearVelChange = glm::cross(AngularVelChange, m_RelativeContactPosition2);
		DeltaVelocity_PerUnitImpulse = glm::dot(LinearVelChange, m_ContactNormal);
		// Add the linear component of velocity change
		DeltaVelocity_PerUnitImpulse += m_RigidBody2->m_InverseMass;

		/** Step 4 **/
		ContactImpulse.x = DeltaVelocity / DeltaVelocity_PerUnitImpulse;
		WorldImpulse = - m_ContactToWorld * ContactImpulse;

		/** Step 5 **/
		VelocityChange = WorldImpulse * m_RigidBody2->m_InverseMass;
		ImpulsiveTorque = glm::cross(WorldImpulse, m_ContactPoint - m_RigidBody2->m_Position);
		RotationChange = m_RigidBody2->m_InverseTensorWorld * ImpulsiveTorque;

		m_RigidBody2->m_Velocity += VelocityChange;
		m_RigidBody2->m_AngularVelocity += RotationChange;
	}
}