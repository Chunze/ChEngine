#include "StaticMeshComponent.h"
#include "PhysicsManager.h"

StaticMeshComponent::StaticMeshComponent(GameContext* gameObject, World* world)
	: Super(gameObject, world)
{
	InitPhysicsParticle();
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
