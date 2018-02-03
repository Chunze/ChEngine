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

	void UpdateForces(float Delta);

	void Update(float Delta);

	void SetIntegrator(int in) { m_intergrator = in; }

protected:
	// 0 = Euler; 1 = RK4
	int m_intergrator;
};
#endif