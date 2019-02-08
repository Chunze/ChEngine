#include <vector>
#include "Contacts.h"
#include "RigidBody.h"

struct CollisionInfo
{
	// allows at most 4 contacts for two collision shape
	std::vector<BodyContact> m_Contacts;

	size_t m_MaxContactCount = 4;

	float m_Friction;
	float m_Restitution;

	void AddContact(BodyContact Contact)
	{
		if (m_Contacts.size() >= 4)
		{
			return;
		}

		for (auto &it : m_Contacts)
		{
			if (it == Contact)
			{
				return;
			}
		}

		m_Contacts.push_back(Contact);
	}

};