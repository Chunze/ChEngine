#include <vector>
#include "Contacts.h"
#include "RigidBody.h"

struct CollisionInfo
{
	// allows at most 4 contacts for two collision shape
	std::vector<BodyContact> m_Contacts;
	BodyContact m_FinalContact;

	static const size_t m_MaxContactCount = 4;

	float m_Friction;
	float m_Restitution;

	void AddContact(BodyContact Contact)
	{
		if (m_Contacts.size() >= m_MaxContactCount)
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

	/// If there are more than one contacts, combine them into one.
	/// Picking largest penetration, average the normal and contact point.
	void CombineContacts();

	bool m_IsValid = false;

	void ResolveInterPenetration(vec3 LinearChange[2], vec3 AngularChange[2]);
	void ResolveVelocity();

};
