#include "CollisionInfo.h"

BodyContact *CollisionInfo::GetNewContact()
{
	BodyContact NewContact;
	m_Contacts.push_back(NewContact);
	return &m_Contacts.back();
}

void CollisionInfo::AddContact(BodyContact Contact, bool bUpdateFeature)
{
	m_Contacts.push_back(Contact);

	if (bUpdateFeature)
	{
		m_ContactFeatureList.UpdateFeatureList(Contact.m_ContactFeature);
	}
}

void CollisionInfo::clearContacts()
{
	m_Contacts.clear();
}

void CollisionInfo::CleanupFeatureList()
{
	m_ContactFeatureList.CleanupFeatureList();
}
