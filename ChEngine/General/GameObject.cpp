#include "GameObject.h"

GameObject::GameObject(GameContext* gameContext, World* world, glm::vec3 location, quat rotation)
	: Super(gameContext, world, location, rotation)
{

}

void GameObject::RenderObject()
{
	CreateAndAddDrawListElement(0);
}

void GameObject::Update(float Delta)
{
	//Super::Update(Delta);
	UpdateTransform();
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

void GameObject::AddComponent(Component* ComponentToAdd)
{
	for (Component* component : m_Components)
	{
		if (ComponentToAdd == component)
		{
			return;
		}
	}
	m_Components.push_back(ComponentToAdd);
	m_Children.push_back(ComponentToAdd);
	ComponentToAdd->SetOwner(this);
}

void GameObject::SetRootComponent(Component* RootComponent)
{
	AddComponent(RootComponent);

	m_RootComponent = RootComponent;
	RootComponent->SetIsRoot(true);
	m_RootComponent->SetWorldTransform(GetWorldTransform());
}

void GameObject::CreateAndAddDrawListElement(int Mode)
{
	for (Component* Comp : m_Components)
	{
		Comp->AddDrawListElement();
	}
}
