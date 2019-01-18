#pragma once
#include "SceneObject.h"

class GameObject;

class Component : public SceneObject
{
	typedef SceneObject Super;
public:
	Component(GameContext* gameContext);
	Component(GameContext* gameContext, World* world);
	~Component();

	virtual void AddDrawListElement() {};

};

