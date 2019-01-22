#pragma once
#include "Contacts.h"
#include "Particle.h"

class ParticleContactGenerator
{
public:

	virtual int AddContact(ParticleContact* contact, unsigned int limit) const = 0;
};