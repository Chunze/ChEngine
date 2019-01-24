#pragma once
#include <memory>
#include "Contacts.h"
#include "Particle.h"

class ParticleContactGenerator
{
public:

	virtual int AddContact(std::shared_ptr<ParticleContact> contact, unsigned int limit) const = 0;
};