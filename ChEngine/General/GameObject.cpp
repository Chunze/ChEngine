#include "GameObject.h"

GameObject::GameObject(GameContext* gameContext, World* world)
	: BaseClass(gameContext),
	m_world(world)
{

}