#pragma once
#include "BaseClass.h"

class Component : public BaseClass
{
public:
	Component(GameContext* gameContext);
	~Component();

	virtual void AddDrawListElement() {};
};

