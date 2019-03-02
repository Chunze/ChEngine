#include "RigidBox.h"
#include "RigidSphere.h"
#include "StaticMeshComponent.h"
#include "PhysicsManager.h"

StaticMeshComponent::StaticMeshComponent(GameContext* gameObject, World* world)
	: Super(gameObject, world)
{
	InitPhysicsBody();
}

std::vector<DrawListElement> StaticMeshComponent::GetherDrawCall() const
{
	if (m_Model != nullptr)
	{
		auto DrawElements = m_Model->GetDrawListElement();
		for (auto& element : DrawElements)
		{
			element.worldTransform = m_WorldTransform;
		}

		return DrawElements;
	}

	return std::vector<DrawListElement>();
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
// 	m_RigidBody = std::make_shared<RigidSphere>(1.0);
// 		m_RigidBody = std::make_shared<RigidBox>(vec3(1.0f));
// 		m_gameContext->GetPhysicsManager()->RegisterPhysicsBody(m_RigidBody);
// 	
// 		// TODO: the collision primitive should be calculated and attached in mesh or model class
// 		if (bUseCollision)
// 		{
// 			CollisionPrimitive_sp Collision = std::make_shared<SpherePrimitive>(1.0f);
// 	
// 			CollisionPrimitive_sp Collision = std::make_shared<BoxPrimitive>(vec3(1.0f));
// 	
// 			m_RigidBody->AddCollisionPrimitive(Collision);
// 		}
}
