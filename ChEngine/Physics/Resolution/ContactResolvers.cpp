#include "Contacts.h"
#include "RigidBody.h"


const float BodyContact::m_Restitution = 0.4f;
const float BodyContact::m_VelocityLimit = 0.05f;
const float BodyContact::m_AngularLimit = 0.2f;
const float BodyContact::m_EnergyLoss = 0.7f;

void BodyContact::ResolveVelocity(/*float duration*/)
{
	//ResolveInterpenetration();

	/** Step 1: construct the contact coordinate axis **/

	ConstructContactToWorld();

	/** Step 2: calculate change in velocity of the contact point on each object per unit of impulse. **/

	// Linear velocity change caused by angular velocity change per unit of impulse
	// this was calculated when resolving penetration
	float DeltaVelocity_PerUnitImpulse = m_AngularInertia[0];

	// Add the linear component of velocity change
	DeltaVelocity_PerUnitImpulse += m_RigidBody[0]->m_InverseMass;

	/** Step 2: calculate closing velocity in contact space **/

	vec3 ContactVelocity = m_WorldToContact * m_RigidBody[0]->GetLinearVelocity(m_ContactPoint);
	if (m_RigidBody[1])
	{
		ContactVelocity -= m_WorldToContact * m_RigidBody[1]->GetLinearVelocity(m_ContactPoint);
	}

	// Check if the two points are closing on each other
	if (ContactVelocity.x > 0)
	{
		return;
	}

	m_TheRestitution = glm::dot(ContactVelocity, ContactVelocity) < m_VelocityLimit * m_VelocityLimit ? 0.0f : m_Restitution;

	float DeltaVelocity = -ContactVelocity.x * (1 + m_TheRestitution) * m_EnergyLoss;

	/** Step 3: calculate the impulse needed to result the change in step 2 **/

	vec3 ContactImpulse, WorldImpulse;
	ContactImpulse.x = DeltaVelocity / DeltaVelocity_PerUnitImpulse;
	WorldImpulse = m_ContactToWorld * ContactImpulse;

	/** step 4: split the impulse into linear and angular components **/

	// Linear velocity change
	vec3 VelocityChange = WorldImpulse * m_RigidBody[0]->m_InverseMass;
	// Angular velocity change
	vec3 ImpulsiveTorque = glm::cross(m_RelativeContactPosition[0], WorldImpulse);
	vec3 RotationChange = m_RigidBody[0]->m_InverseTensorWorld * ImpulsiveTorque;

	m_RigidBody[0]->m_Velocity += VelocityChange;
	m_RigidBody[0]->m_AngularVelocity += RotationChange;

	
	if (m_RigidBody[1])
	{
		/** Step 2 **/
		DeltaVelocity_PerUnitImpulse = m_AngularInertia[1];
		// Add the linear component of velocity change
		DeltaVelocity_PerUnitImpulse += m_RigidBody[1]->m_InverseMass;

		/** Step 3 **/
		ContactImpulse.x = DeltaVelocity / DeltaVelocity_PerUnitImpulse;
		WorldImpulse = - m_ContactToWorld * ContactImpulse;

		/** Step 4 **/
		VelocityChange = WorldImpulse * m_RigidBody[1]->m_InverseMass;
		ImpulsiveTorque = glm::cross(m_RelativeContactPosition[1], WorldImpulse);
		RotationChange = m_RigidBody[1]->m_InverseTensorWorld * ImpulsiveTorque;

		m_RigidBody[1]->m_Velocity += VelocityChange;
		m_RigidBody[1]->m_AngularVelocity += RotationChange;
	}
}



void BodyContact::ResolveInterpenetration(vec3 LinearChange[2], vec3 AngularChange[2])
{
	if (m_Penetration <= 0)
	{
		// There isn't any penetration
		return;
	}

	// Using non-linear resolving, linear movement + angular movement
	float AngularMove[2];
	float LinearMove[2];
	float TotalInertia = 0.0f;
	float LinearInertia[2];

	// calculate TotalInertia
	CalculateAngularInertia();
	LinearInertia[0] = m_RigidBody[0]->m_InverseMass;

	TotalInertia = m_AngularInertia[0] + LinearInertia[0];
	if (m_RigidBody[1])
	{
		LinearInertia[1] = m_RigidBody[1]->m_InverseMass;
		TotalInertia += m_AngularInertia[1] + LinearInertia[1];
	}

	for (unsigned i = 0; i < 2; i++) if (m_RigidBody[i])
	{
		// The linear and angular movements required are in proportion to
		// the two inverse inertias.
		float sign = (i == 0) ? 1.0f : -1.0f;
		float Multiplier = sign * m_Penetration / TotalInertia;
		AngularMove[i] = Multiplier * m_AngularInertia[i];
		LinearMove[i] = Multiplier * LinearInertia[i];

		// To avoid angular projections that are too great (when mass is large
		// but inertia tensor is small) limit the angular move.
		float ProjectionMag = glm::length(m_RelativeContactPosition[i] + glm::dot(m_RelativeContactPosition[i], m_ContactNormal) * m_ContactNormal);

		// Use the small angle approximation for the sine of the angle
		// approximate sine(angularLimit) to angularLimit
		float MaxMagnitude = m_AngularLimit * ProjectionMag;
		float TotalMove = AngularMove[i] + LinearMove[i];
		if (AngularMove[i] < -MaxMagnitude)
		{
			AngularMove[i] = -MaxMagnitude;
			LinearMove[i] = TotalMove - AngularMove[i];
		}
		else if (AngularMove[i] > MaxMagnitude)
		{
			AngularMove[i] = MaxMagnitude;
			LinearMove[i] = TotalMove - AngularMove[i];
		}

		// AngularMove is the linear amount of movement required by turning the rigid body.
		// calculate the desired rotation to achieve that.
		if (abs(AngularMove[i]) < SMALL_NUMBER)
		{
			AngularChange[i].x = 0.0f;
			AngularChange[i].y = 0.0f;
			AngularChange[i].z = 0.0f;
		}
		else
		{
			vec3 TargetAngularDirection = glm::cross(m_RelativeContactPosition[i], m_ContactNormal);
			AngularChange[i] = (m_RigidBody[i]->m_InverseInertiaTensor * TargetAngularDirection) * AngularMove[i] / m_AngularInertia[i];
		}

		LinearChange[i] = m_ContactNormal * LinearMove[i];

		// apply linear change
		m_RigidBody[i]->AddPosition(LinearChange[i]);

		// apply angular change
		m_RigidBody[i]->AddRotation(AngularChange[i]);
	}
}

void BodyContact::CalculateAngularInertia()
{
	for (int i = 0; i < 2; i++) if (m_RigidBody[i])
	{
		vec3 Torque = glm::cross(m_RelativeContactPosition[i], m_ContactNormal);
		vec3 AngularVelChange = m_RigidBody[i]->m_InverseInertiaTensor * Torque;
		vec3 LinearVelChange = glm::cross(AngularVelChange, m_RelativeContactPosition[i]);
		// Change in velocity in contact space
		m_AngularInertia[i] = glm::dot(LinearVelChange, m_ContactNormal);
	}
}
