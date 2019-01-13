#include "GameObject.h"

GameObject::GameObject(GameContext* gameContext, World* world)
	: BaseClass(gameContext),
	m_world(world)
{

}

void GameObject::CreateAndAddDrawListElement(int Mode)
{
	for (Component* Comp : m_Components)
	{
		Comp->AddDrawListElement();
	}
}
