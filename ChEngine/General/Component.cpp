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

Component::~Component()
{
}
