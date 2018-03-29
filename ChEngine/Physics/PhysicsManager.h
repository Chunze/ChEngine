#ifndef PHYSICS_MANAGER_H
#define PHYSICS_MANAGER_H
#include <vector>

#include "BaseClass.h"
#include "ParticleForceRegistry.h"
#include "glm.h"

class Particle;
class ForceGenerator;
class World;

class PhysicsManager : public BaseClass
{
public:

	PhysicsManager(GameContext* gameContext);

	ParticleForceRegistry m_particleForceRegistry;

	ParticleForceRegistry m_contactForceRegistry;

	void registerForce(Particle* particle, ForceGenerator* FG);

	// updating constant forces
	void UpdateForces(float Delta);

	// updating forces for one frame, new contact forces will be generated next frame
	void UpdateContactForces(float Delta);

	void Update_RK4(float Delta);


	// Generating collision spring
	void GenerateCollisionInfo(Particle* particle, Particle* Anchor, glm::vec3 OutwardDirection, float _springConstant, float _damping);

	void AddPhysicsParticle(Particle* ParticleToAdd);

	int GetIntegrator() { return m_intergrator; }
	void SetIntegrator(int in) { m_intergrator = in; }

	int GetRK4StepCount() { return RK4_step; }

	void Increment_RK4_step() { RK4_step++; }
	void Reset_RK4_Step() { RK4_step = 1; }
	
protected:
	// 0 = Euler; 1 = RK4
	int m_intergrator;

	// RK4 step count
	int RK4_step = 1;
	std::vector<Particle*> m_physicsParticles;
	World* m_world;
};
#endif