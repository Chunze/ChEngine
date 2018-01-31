#pragma once
#include "World.h"

class GameObject
{
public:
	GameObject() {}
	GameObject(World* world);
	World* m_world;

	virtual void DrawObject(int Mode) = 0;

	World* GetWorld() { return m_world; }

};