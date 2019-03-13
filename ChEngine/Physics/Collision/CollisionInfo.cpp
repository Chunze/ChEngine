#include "CollisionInfo.h"

BodyContact *CollisionInfo::GetNewContact()
{
	BodyContact NewContact;
	m_Contacts.push_back(NewContact);
	return &m_Contacts.back();
}

void CollisionInfo::AddContact(BodyContact Contact)
{
	m_Contacts.push_back(Contact);
}

void CollisionInfo::clearContacts()
{
	m_Contacts.clear();
}
