#include "Component.h"
#include "GameObject.h"


Component::Component(GameContext* gameContext)
	: Super(gameContext)
{
}


Component::Component(GameContext* gameContext, World* world)
	: Super(gameContext, world)
{

}

void Component::RemoveChild(Component_sp child)
{

}

void Component::SetOwner(GameObject* owner)
{
	if (m_Owner == nullptr)
	{
		m_Owner = owner;
	}
}

void Component::Update(float Delta)
{

}

void Component::UpdateTransform()
{
}

Component::~Component()
{
}
