#ifndef WORLD_H
#define WORLD_H
#pragma once
#include <vector>
#include "BaseClass.h"
#include "DrawList.h"
#include "Camera.h"
#include "Renderer.h"
#include "Game.h"

class GameObject;
class SceneObject;

class World : public BaseClass
{
public:
	World(GameContext* gameContext);

	virtual ~World() = 0
	{
		
	}

	virtual bool LoadWorld(const char* fileName) { return false; }
	virtual void Update(float Delta);
	virtual void RenderWorld();

	float m_customDelta = -1.0f;

protected:
	SceneObject* m_RootSceneObject;
	std::vector<SceneObject*> m_SceneObjects;
	Camera* m_Camera;
	virtual void InitCamera() = 0;
	virtual void Init();
};

#endif
