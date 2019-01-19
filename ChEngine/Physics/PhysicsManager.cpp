#include "PhysicsManager.h"
#include "JelloWorld.h"
#include "ForceGenerators/CollisionSpringFG.h"

PhysicsManager::PhysicsManager(GameContext* gameContext)
	: BaseClass(gameContext)
{

}

void PhysicsManager::registerForce(Particle* particle, ForceGenerator* FG)
{
	m_particleForceRegistry.Add(particle, FG);
}

void PhysicsManager::Update(float Delta)
{
	m_Damping = powf(m_DampingCoef, Delta);

	UpdateForces(Delta);

	Integrate(Delta);
}

void PhysicsManager::Integrate(float Delta)
{
	for (Particle* particle : m_physicsParticles)
	{
		particle->Integrate(Delta);
	}
}

void PhysicsManager::UpdateForces(float Delta)
{
	m_particleForceRegistry.UpdateForces(Delta);
}

void PhysicsManager::UpdateContactForces(float Delta)
{
	m_contactForceRegistry.UpdateForces(Delta);

	m_contactForceRegistry.Clear();
}

void PhysicsManager::GenerateCollisionInfo(Particle* particle, Particle* Anchor, glm::vec3 OutwardDirection, float _springConstant, float _damping)
{
	CollisionSpringFG* collisionFG = new CollisionSpringFG(Anchor, OutwardDirection, _springConstant, _damping);

	m_contactForceRegistry.Add(particle, collisionFG);
}

void PhysicsManager::AddPhysicsParticle(Particle* ParticleToAdd)
{
	m_physicsParticles.push_back(ParticleToAdd);
	ParticleToAdd->SetPhysicsManager(this);
}
