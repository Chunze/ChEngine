#pragma once
#include "SceneObject.h"

class GameObject;

class Component : public SceneObject
{
	typedef SceneObject Super;
public:
	Component(GameContext* gameContext);
	~Component();

	virtual void AddDrawListElement() {};

};

