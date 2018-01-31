#ifndef WORLD_H
#define WORLD_H
#pragma once
#include "BaseClass.h"

class World : BaseClass
{
public:
	World(GameContext gameContext)
		: BaseClass(gameContext)
	{}

	virtual void LoadWorld(const char* fileName) = 0;
	virtual void Update(float Delta) = 0;
};

#endif
