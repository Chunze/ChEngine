#include <vector>
#include "BodyContactData.h"

struct CollisionInfo
{
	// allows at most 4 contacts for two collision shape
	std::vector<BodyContact> m_Contacts;

	// Stores contact features from the past frames
	ContactFeatureList m_ContactFeatureList;

	float m_Friction;
	float m_Restitution;

	BodyContact *GetNewContact();

	void AddContact(BodyContact Contact, bool bUpdateFeature = true);
	void clearContacts();
	void CleanupFeatureList();

	bool HasCollision() { return m_Contacts.size() > 0; }
	unsigned int ContactCount() { return m_Contacts.size(); }

	bool m_IsValid = false;
};
