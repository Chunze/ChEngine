#include "CollisionInfo.h"

void CollisionInfo::CombineContacts()
{
	if (m_IsValid)
	{
		return;
	}

	if (m_Contacts.size() == 0)
	{
		m_IsValid = false;
		return;
	}
	else if (m_Contacts.size() > 1)
	{
		m_FinalContact.SetBodies(m_Contacts[0].m_RigidBody[0], m_Contacts[0].m_RigidBody[1]);

		vec3 NewContactPoint(0.0f);
		vec3 NewContactNormal(0.0f);
		float NewPenetration = -1.0f;
		for (size_t index = 0; index < m_Contacts.size(); index++)
		{
			NewContactPoint += m_Contacts[index].m_ContactPoint;
			NewContactNormal += m_Contacts[index].m_ContactNormal;
			if (m_Contacts[index].m_Penetration > NewPenetration)
			{
				NewPenetration = m_Contacts[index].m_Penetration;
			}
		}

		NewContactPoint /= (float)m_Contacts.size();
		NewContactNormal /= (float)m_Contacts.size();

		m_FinalContact.SetContactPoint(NewContactPoint);
		m_FinalContact.SetContactNormal(NewContactNormal);
		m_FinalContact.SetContactPenetration(NewPenetration);
	}
	else if (m_Contacts.size() == 1)
	{
		m_FinalContact = m_Contacts[0];
	}

	m_IsValid = true;
}

void CollisionInfo::ResolveInterPenetration(vec3 &BodyMovement1, vec3 &BodyMovement2)
{
	m_FinalContact.ResolveInterpenetration(BodyMovement1, BodyMovement2);
}

void CollisionInfo::ResolveVelocity()
{
	if (m_IsValid)
	{
		m_FinalContact.ResolveVelocity();
	}
}