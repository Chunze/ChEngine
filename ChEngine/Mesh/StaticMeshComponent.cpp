#include "StaticMeshComponent.h"



StaticMeshComponent::StaticMeshComponent(GameContext* gameObject, World* world)
	: Super(gameObject, world)
{
}


void StaticMeshComponent::AddDrawListElement()
{
	if (m_Model != nullptr)
	{
		auto DrawElements = m_Model->GetDrawListElement();
		for (auto element : DrawElements)
		{
			element.worldTransform = m_WorldTransform;
			m_gameContext->GetDrawList()->AddToDrawQ(element);
		}
	}
}

StaticMeshComponent::~StaticMeshComponent()
{
}
