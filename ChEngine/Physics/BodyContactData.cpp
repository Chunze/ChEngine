#include "BodyContactData.h"
#include "MathStatics.h"
#include "RigidBody.h"

bool BodyContact::ConstructContactToWorld()
{
	// TODO: assume there is only one contact point for now
	vec3 x = m_ContactNormal;
	vec3 y(0.0f, 1.0f, 0.0f);
	vec3 z;
	// To make the calculation stable, choose X-axis as the second basis if x is near Y-axis
	if (glm::abs(x.x) <= glm::abs(x.y))
	{
		y.x = 1.0f;
		y.y = 0.0f;
	}

	bool bSuccessful = Math::MakeOrthonormalBasis(x, y, z);

	if (bSuccessful)
	{
		m_ContactToWorld = mat3(x, y, z);
		m_WorldToContact = glm::transpose(m_ContactToWorld);
	}

	return bSuccessful;
}

void BodyContact::SetContactPoint(vec3 ContactPoint)
{
	m_ContactPoint = ContactPoint;
	m_RelativeContactPosition[0] = m_ContactPoint - m_RigidBody[0]->GetPosition();
	if (m_RigidBody[1])
	{
		m_RelativeContactPosition[1] = m_ContactPoint - m_RigidBody[1]->GetPosition();
	}
}

void BodyContact::SetContactNormal(vec3 ContactNormal)
{
	m_ContactNormal = glm::normalize(ContactNormal);
}

void BodyContact::SetContactPenetration(float Penetration)
{
	m_Penetration = Penetration;
}

float BodyContact::GetTotalInverseMass()
{
	float TotalInverseMass = m_RigidBody[0]->m_InverseMass;
	TotalInverseMass += m_RigidBody[1] ? m_RigidBody[1]->m_InverseMass : 0;

	return TotalInverseMass;
}