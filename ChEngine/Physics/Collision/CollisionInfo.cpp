#include "CollisionInfo.h"

void CollisionInfo::Resolve()
{
	auto ContactNum = m_Contacts.size();

	if (ContactNum == 0)
	{
		return;
	}
	else if (ContactNum == 1)
	{
		m_Contacts[0].Resolve();
	}
	else if (ContactNum > 1)
	{
		BodyContact Contact;
		Contact.SetBodies(m_Contacts[0].m_RigidBody1, m_Contacts[0].m_RigidBody2);

		vec3 NewContactPoint(0.0f);
		vec3 NewContactNormal(0.0f);
		float NewPenetration = -1.0f;
		for (size_t index = 0; index < ContactNum; index++)
		{
			NewContactPoint += m_Contacts[index].m_ContactPoint;
			NewContactNormal += m_Contacts[index].m_ContactNormal;
			if (m_Contacts[index].m_Penetration > NewPenetration)
			{
				NewPenetration = m_Contacts[index].m_Penetration;
			}
		}

		NewContactPoint /= (float)ContactNum;
		NewContactNormal /= (float)ContactNum;

		Contact.SetContactPoint(NewContactPoint);
		Contact.m_ContactNormal = NewContactNormal;
		Contact.m_Penetration = NewPenetration;
		Contact.Resolve();
	}
}