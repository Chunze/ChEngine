#ifndef WORLD_H
#define WORLD_H
#pragma once
#include "BaseClass.h"
#include "DrawList.h"
#include "Camera.h"
#include "Renderer.h"
#include "Game.h"

class World : public BaseClass
{
public:
	World(GameContext* gameContext)
		: BaseClass(gameContext)
	{}

	virtual ~World() = 0
	{

	}

	virtual bool LoadWorld(const char* fileName) { return false; }
	virtual void Update(float Delta) = 0;

	float m_customDelta = -1.0f;

	//std::vector<GameObject> m_gameObjects;

protected:
	Camera* m_Camera;
	virtual void InitCamera() = 0;
};

#endif
