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
class IPhysicsProxy;


class World : public BaseClass
{
public:
	World(GameContext* gameContext);

	virtual ~World() = 0
	{
		
	}

	virtual bool LoadWorld(const char* fileName) { return false; }
	virtual void Update(float Delta);
	virtual void PostPhysicsUpdate();

	GameObjects GetAllGameObjects() { return m_GameObjects; }

	void RegisterGameObject(GameObject_sp Object);
	void RegisterComponent(Component_sp Component);

	PhysicsProxies GetPhysicsProxies();
	RenderableObjects GetRenderableObjects();

	float m_customDelta = -1.0f;

protected:
	GameObjects m_GameObjects;
	Components m_Components;

	Camera* m_Camera;
	virtual void InitCamera() = 0;
	virtual void Init();
};

#endif
