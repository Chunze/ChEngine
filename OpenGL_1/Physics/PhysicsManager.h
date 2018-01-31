#ifndef PHYSICS_MANAGER_H
#define PHYSICS_MANAGER_H

#include "ParticleForceRegistry.h"

class Particle;
class ForceGenerator;

class PhysicsManager
{
public:
	PhysicsManager();

	ParticleForceRegistry m_particleForceRegistry;

	void registerForce(Particle* particle, ForceGenerator* FG);

};
#endif