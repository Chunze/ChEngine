#include "PhysicsManager.h"
#include "JelloWorld.h"
#include "Contacts.h"
#include "ForceGenerators/CollisionSpringFG.h"

PhysicsManager::PhysicsManager(GameContext* gameContext)
	: BaseClass(gameContext)
{
	Init();
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

	RunCollisionDetection();

	RunCollisionResolution(Delta);
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

void PhysicsManager::Init()
{
	m_ParticleContactResolver = new ParticleContactResolver(10);
}

void PhysicsManager::RunCollisionDetection()
{
	for (Particle* particle : m_physicsParticles)
	{
		if (particle->GetPosition().y < m_PlaneHeight)
		{
			ParticleContact* contact = new ParticleContact();
			contact->m_Particles[0] = particle;
			contact->m_Penetration = m_PlaneHeight - particle->GetPosition().y;
			contact->m_ContactNormal = glm::vec3(0.0f, 1.0f, 0.0f);
			contact->m_Restitution = 0.5f;

			m_ParticleContacts.push_back(*contact);
		}
	}
}

void PhysicsManager::RunCollisionResolution(float Delta)
{
	if (m_ParticleContactResolver != nullptr && m_ParticleContacts.size() > 0)
	{
		m_ParticleContactResolver->ResolveContacts(m_ParticleContacts, Delta);
		m_ParticleContacts.clear();
	}
}
