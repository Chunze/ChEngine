#include "PhysicsManager.h"
#include "Contacts.h"
#include "ForceGenerators/CollisionSpringFG.h"
#include "IPhysicsProxy.h"
#include "ParticleContactGenerator.h"
#include "RigidBody.h"

PhysicsManager::PhysicsManager(GameContext* gameContext)
	: BaseClass(gameContext),
	  m_CollisionDetection(this),
	  m_CollisionResolution(this)
{
	Init();
}

void PhysicsManager::registerForce(Particle* particle, ForceGenerator* FG)
{
	m_particleForceRegistry.Add(particle, FG);
}

void PhysicsManager::Update(float Delta)
{
	m_LinearDamping = powf(m_LinearDampingCoef, Delta);
	m_AngularDamping = powf(m_AngularDampingCoef, Delta);

	UpdateForces(Delta);

	Integrate(Delta);

	m_CollisionDetection.RunCollisionDetection();
	RunCollisionDetection();

	m_CollisionResolution.RunCollisionResolution(Delta);
	RunCollisionResolution(Delta);
}

void PhysicsManager::Integrate(float Delta)
{
	for (auto particle : m_physicsParticles)
	{
		particle->Integrate(Delta);
	}

	for (auto rigid_body : m_RigidBodies)
	{
		rigid_body->Integrate(Delta);
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

void PhysicsManager::ConstructPhysicsScene(PhysicsProxies Proxies)
{
	for (IPhysicsProxy* Proxy : Proxies)
	{
		auto rigidBody = Proxy->GetRigidBody();
		if (rigidBody != nullptr)
		{
			RegisterPhysicsBody(rigidBody);
		}
	}

	for (IPhysicsProxy* Proxy : Proxies)
	{
		auto collisionPrimitive = Proxy->GetCollisionPrimitive();
		if (collisionPrimitive != nullptr)
		{
			RegisterCollisionPrimitive(collisionPrimitive);
		}
	}
}

void PhysicsManager::GenerateCollisionInfo(Particle* particle, Particle* Anchor, glm::vec3 OutwardDirection, float _springConstant, float _damping)
{
	CollisionSpringFG* collisionFG = new CollisionSpringFG(Anchor, OutwardDirection, _springConstant, _damping);

	m_contactForceRegistry.Add(particle, collisionFG);
}

void PhysicsManager::AddPhysicsParticle(Particle_sp ParticleToAdd)
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

void PhysicsManager::RegisterPhysicsBody(RigidBody_sp BodyToAdd)
{
	for (auto body : m_RigidBodies)
	{
		if (body == BodyToAdd)
		{
			return;
		}
	}
	m_RigidBodies.push_back(BodyToAdd);
	BodyToAdd->SetPhisicsManager(this);
}

void PhysicsManager::RegisterCollisionPrimitive(CollisionPrimitive_sp PrimitiveToAdd)
{
	m_CollisionDetection.RegisterCollisionPrimitive(PrimitiveToAdd);
}

void PhysicsManager::Init()
{
	//m_ParticleContactResolver = new ParticleContactResolver(10);
	m_ParticleContactResolver = std::make_unique<ParticleContactResolver>(0);
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
		if (!contactGenerator->AddContact(contact, 1))
		{
			m_ParticleContacts.pop_back();
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
