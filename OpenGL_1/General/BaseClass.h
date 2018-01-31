#pragma once
#include "GameContext.h"


class BaseClass
{
public:
	BaseClass(GameContext gameContext)
		: m_gameContext(gameContext)
	{}

	GameContext m_gameContext;
};