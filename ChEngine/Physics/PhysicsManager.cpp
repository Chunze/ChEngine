#include "PhysicsManager.h"
#include "JelloWorld.h"
#include "Contacts.h"
#include "ForceGenerators/CollisionSpringFG.h"
#include "ParticleContactGenerator.h"

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
	for (auto particle : m_physicsParticles)
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

void PhysicsManager::AddPhysicsParticle(shared_ptr<Particle> ParticleToAdd)
{
	m_physicsParticles.push_back(ParticleToAdd);
	ParticleToAdd->SetPhysicsManager(this);
}

void PhysicsManager::AddParticleContactGenerator(ParticleContactGenerator* contactGenerator)
{
	if (contactGenerator != nullptr)
	{
		m_ContactGenerator.push_back(contactGenerator);
	}
}

void PhysicsManager::Init()
{
	//m_ParticleContactResolver = new ParticleContactResolver(10);
	m_ParticleContactResolver = std::make_unique<ParticleContactResolver>(10);
}

void PhysicsManager::RunCollisionDetection()
{
	for (auto particle : m_physicsParticles)
	{
		if (particle->GetPosition().y < m_PlaneHeight)
		{
			auto contact = make_shared<ParticleContact>();
			contact->m_Particle_1 = particle;
			contact->m_Penetration = m_PlaneHeight - particle->GetPosition().y;
			contact->m_ContactNormal = glm::vec3(0.0f, 1.0f, 0.0f);
			contact->m_Restitution = 0.5f;
			contact->SetIsValid(true);

			m_ParticleContacts.push_back(contact);
		}
	}

	for (ParticleContactGenerator* contactGenerator : m_ContactGenerator)
	{
		if (m_ParticleContacts.size() == 0 || m_ParticleContacts.back()->IsValid())
		{
			auto contact = make_shared<ParticleContact>();
			m_ParticleContacts.push_back(contact);
		}

		auto contact = m_ParticleContacts.back();
		contactGenerator->AddContact(contact, 1);
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
