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

	PhysicsManager(GameContext gameContext);

	ParticleForceRegistry m_particleForceRegistry;

	ParticleForceRegistry m_contactForceRegistry;

	void registerForce(Particle* particle, ForceGenerator* FG);

	// updating constant forces
	void UpdateForces(float Delta);

	// updating forces for one frame, new contact forces will be generated next frame
	void UpdateContactForces(float Delta);

	void Update(float Delta);


	// Generating collision spring
	void GenerateCollisionInfo(Particle* particle, Particle* Anchor, glm::vec3 OutwardDirection, float _springConstant, float _damping);

	void AddPhysicsParticle(Particle* ParticleToAdd);

	int GetIntegrator() { return m_intergrator; }
	void SetIntegrator(int in) { m_intergrator = in; }
	
protected:
	// 0 = Euler; 1 = RK4
	int m_intergrator;
	std::vector<Particle*> m_physicsParticles;
	World* m_world;
};
#endif