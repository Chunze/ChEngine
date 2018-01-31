#pragma once
#include "World.h"
#include "BaseClass.h"

class GameObject : BaseClass
{
public:
	GameObject(GameContext gameContext, World* world);
	World* m_world;

	virtual void DrawObject(int Mode) = 0;

	World* GetWorld() { return m_world; }

};