#include "Component.h"
#include "GameObject.h"


Component::Component(GameContext* gameContext)
	: BaseClass(gameContext)
{
}


Component::~Component()
{
}

void Component::Update(float Delta)
{
	m_WorldTransform = Owner->GetWorldTransform() * m_RaletiveTransform;
}
