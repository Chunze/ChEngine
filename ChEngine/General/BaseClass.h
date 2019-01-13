#pragma once
#include "GameContext.h"
#include "glm.h"

class BaseClass
{
public:
	BaseClass() {}

	BaseClass(GameContext* gameContext)
		: m_gameContext(gameContext)
	{}

	~BaseClass()
	{

	}

	GameContext* m_gameContext;
};