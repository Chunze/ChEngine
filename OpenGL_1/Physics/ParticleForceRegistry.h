#ifndef PARTICLE_FORCE_REGISTRY_H
#define PARTICLE_FORCE_REGISTRY_H
#pragma once

#include <vector>

class Particle;
class ForceGenerator;

class ParticleForceRegistry
{
public:
	ParticleForceRegistry();

	/**
	* Registers the given force generator to apply to the
	* given particle.
	*/
	void Add(Particle* particle, ForceGenerator* fg);
	/**
	* Removes the given registered pair from the registry.
	* If the pair is not registered, this method will have
	* no effect.
	*/
	void Remove(Particle* particle, ForceGenerator* fg);
	/**
	* Clears all registrations from the registry. This will
	* not delete the particles or the force generators
	* themselves, just the records of their connection.
	*/
	void Clear();
	/**
	* Calls all the force generators to update the forces of
	* their corresponding particles.
	*/
	void UpdateForces(float duration);

protected:

	struct ParticleForceRegistration
	{
		ParticleForceRegistration(Particle* _particle, ForceGenerator* _FG)
			: particle(_particle), FG(_FG)
		{}

		Particle* particle;
		ForceGenerator* FG;

	};

	std::vector<ParticleForceRegistration> registrations;

};


#endif // 
