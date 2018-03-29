#ifndef WORLD_H
#define WORLD_H
#pragma once
#include "BaseClass.h"
#include "DrawList.h"
#include "Camera.h"
#include "Renderer.h"

class World : public BaseClass
{
public:
	World(GameContext* gameContext)
		: BaseClass(gameContext)
	{}

	virtual ~World() = 0
	{

	}

	virtual bool LoadWorld(const char* fileName) = 0;
	virtual void Update(float Delta) = 0;

	//std::vector<GameObject> m_gameObjects;

protected:
	Camera* m_Camera;
	virtual void InitCamera() = 0;
};

#endif
