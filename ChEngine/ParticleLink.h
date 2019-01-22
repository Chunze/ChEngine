#pragma once
#include "ParticleContactGenerator.h"

class ParticleLink : public ParticleContactGenerator
{
public:
	ParticleLink();

	/// Holds the pair of particles that are connected by this links
	Particle* m_Particles[2];

	~ParticleLink();

protected:

	/// Returns the current length of the link
	float CurrentLength() const;
};


class ParticleCableLink : public ParticleLink
{
public:
	float m_MaxLength;

	/// Holds the restitution (bounciness) of the cable
	float m_Restitution;

	virtual int AddContact(ParticleContact* contact, unsigned int limit) const;
};

