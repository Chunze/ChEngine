#pragma once
#include "BaseClass.h"

class GameObject;

class Component : public BaseClass
{
public:
	Component(GameContext* gameContext);
	~Component();

	virtual void AddDrawListElement() {};

	virtual void Update(float Delta);

protected:
	GameObject* Owner;

	// Transform in owner's space
	glm::mat4 m_RaletiveTransform;

	// Transform in world space
	glm::mat4 m_WorldTransform;
};

