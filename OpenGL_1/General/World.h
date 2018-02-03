#ifndef WORLD_H
#define WORLD_H
#pragma once
#include "BaseClass.h"
#include "GameObject.h"
#include <vector>

class World : public BaseClass
{
public:
	World(GameContext gameContext)
		: BaseClass(gameContext)
	{}

	virtual void LoadWorld(const char* fileName) = 0;
	virtual void Update(float Delta) = 0;

	std::vector<GameObject> m_gameObjects;
};

#endif
