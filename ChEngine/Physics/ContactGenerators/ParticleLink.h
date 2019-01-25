#pragma once
#include "ParticleContactGenerator.h"

class ParticleLink : public ParticleContactGenerator
{
public:
	ParticleLink();

	/// Holds the pair of particles that are connected by this links
	std::shared_ptr<Particle> m_Particle_1;
	std::shared_ptr<Particle> m_Particle_2;

	~ParticleLink();

protected:

	/// Returns the current length of the link
	float CurrentLength() const;
};


class ParticleCableLink : public ParticleLink
{
public:
	ParticleCableLink(std::shared_ptr<Particle> particle_1, std::shared_ptr<Particle> particle_2, float MaxLength = 10.0f, float Restitution = 0.8f);

	float m_MaxLength;

	/// Holds the restitution (bounciness) of the cable
	float m_Restitution;

	virtual int AddContact(std::shared_ptr<ParticleContact> contact, unsigned int limit) const;
};

class ParticleRodLink : public ParticleLink
{
public:
	ParticleRodLink(std::shared_ptr<Particle> particle_1, std::shared_ptr<Particle> particle_2, float Length);

	float m_Length;

	virtual int AddContact(std::shared_ptr<ParticleContact> contact, unsigned int limit) const;
};
