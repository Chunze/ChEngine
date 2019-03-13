#include <vector>
#include "Contacts.h"
#include "RigidBody.h"

struct CollisionInfo
{
	// allows at most 4 contacts for two collision shape
	std::vector<BodyContact> m_Contacts;

	float m_Friction;
	float m_Restitution;

	BodyContact *GetNewContact();

	void AddContact(BodyContact Contact);
	void clearContacts();

	bool HasCollision() { return m_Contacts.size() > 0; }
	unsigned int ContactCount() { return m_Contacts.size(); }

	bool m_IsValid = false;
};
