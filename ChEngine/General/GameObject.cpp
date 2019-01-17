#include "GameObject.h"

GameObject::GameObject(GameContext* gameContext, World* world)
	: Super(gameContext, world)
{

}

GameObject::GameObject(GameContext* gameContext, World* world, glm::vec3 location)
	: Super(gameContext, world, location)
{

}

void GameObject::RenderObject()
{
	CreateAndAddDrawListElement(0);
}

void GameObject::Update(float Delta)
{
	Super::Update(Delta);
	for (Component* component : m_Components)
	{
		component->Update(Delta);
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
	ComponentToAdd->SetOwner(this);
}

void GameObject::CreateAndAddDrawListElement(int Mode)
{
	for (Component* Comp : m_Components)
	{
		Comp->AddDrawListElement();
	}
}
