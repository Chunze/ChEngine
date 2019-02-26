#include "World.h"
#include "GameObject.h"
#include "SceneObject.h"

World::World(GameContext* gameContext)
	: BaseClass(gameContext)
{
}

void World::Update(float Delta)
{
	for (SceneObject* sceneObject : m_SceneObjects)
	{
		sceneObject->Update(Delta);
	}
	for (GameObject* gameObject : m_GameObjects)
	{
		gameObject->Update(Delta);
	}
}

void World::PostPhysicsUpdate()
{
	for (SceneObject* sceneObject : m_SceneObjects)
	{
		sceneObject->PostPhysicsUpdate();
	}
}

void World::RenderWorld()
{
	for (GameObject* gameObject : m_GameObjects)
	{
		gameObject->RenderObject();
	}
}

void World::Init()
{
	//m_RootSceneObject = new SceneObject(m_gameContext, this);
}
