#include "RigidBox.h"
#include "RigidSphere.h"
#include "StaticMeshComponent.h"
#include "PhysicsManager.h"

StaticMeshComponent::StaticMeshComponent(GameContext* gameObject, World* world)
	: Super(gameObject, world)
{
	InitPhysicsBody();
}


void StaticMeshComponent::AddDrawListElement()
{
	if (m_Model != nullptr)
	{
		auto DrawElements = m_Model->GetDrawListElement();
		for (auto& element : DrawElements)
		{
			element.worldTransform = m_WorldTransform;
			m_gameContext->GetDrawList()->AddToDrawQ(element);
		}
	}
}

StaticMeshComponent::~StaticMeshComponent()
{
}

void StaticMeshComponent::InitPhysicsParticle()
{
	m_Particle = std::make_shared<Particle>(1.0f, GetWorldLocation(), glm::vec3(0.0f, 0.0f, 0.0f));
	m_gameContext->GetPhysicsManager()->AddPhysicsParticle(m_Particle);
}

void StaticMeshComponent::InitPhysicsBody()
{
	m_RigidBody = std::make_shared<RigidSphere>();
	m_gameContext->GetPhysicsManager()->RegisterPhysicsBody(m_RigidBody);

	// TODO: the collision primitive should be calculated and attached in mesh or model class
	if (bUseCollision)
	{
		CollisionPrimitive_sp Sphere = std::make_shared<SpherePrimitive>(1.0f);
		m_RigidBody->AddCollisionPrimitive(Sphere);
	}
}
