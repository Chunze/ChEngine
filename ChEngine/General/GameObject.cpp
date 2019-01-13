#include "GameObject.h"

GameObject::GameObject(GameContext* gameContext, World* world)
	: BaseClass(gameContext),
	m_world(world)
{

}

GameObject::GameObject(GameContext* gameContext, World* world, glm::vec3 location)
{
	GameObject(gameContext, world);

	SetLocation(location);
}

void GameObject::RenderObject()
{
	CreateAndAddDrawListElement(0);
}

void GameObject::SetLocation(glm::vec3 NewLocation)
{
	m_WorldTransform[4] = glm::vec4(NewLocation, 0);
}

void GameObject::CreateAndAddDrawListElement(int Mode)
{
	for (Component* Comp : m_Components)
	{
		Comp->AddDrawListElement();
	}
}
