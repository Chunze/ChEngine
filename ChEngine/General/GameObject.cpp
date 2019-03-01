#include "GameObject.h"

GameObject::GameObject(GameContext* gameContext, World* world, glm::vec3 location, quat rotation)
	: Super(gameContext, world, location, rotation)
{

}

void GameObject::Update(float Delta)
{

}

void GameObject::UpdateTransform()
{
	if (m_RootComponent != nullptr)
	{
		m_WorldTransform = m_RootComponent->GetWorldTransform();
	}
}

void GameObject::PostPhysicsUpdate()
{
	if (m_RootComponent != nullptr)
	{
		m_WorldTransform = m_RootComponent->GetWorldTransform();
	}
}

void GameObject::AddComponent(Component_sp ComponentToAdd)
{
	for (Component_sp component : m_Components)
	{
		if (ComponentToAdd == component)
		{
			return;
		}
	}
	m_Components.push_back(ComponentToAdd);
	ComponentToAdd->SetOwner(this);
}

void GameObject::SetRootComponent(Component_sp RootComponent)
{
	AddComponent(RootComponent);

	m_RootComponent = RootComponent;
	RootComponent->SetIsRoot(true);
	m_RootComponent->SetWorldTransform(GetWorldTransform());
}
