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
}

void World::RenderWorld()
{
	for (SceneObject* sceneObject : m_SceneObjects)
	{
		GameObject* gameObject = dynamic_cast<GameObject*>(sceneObject);

		if (gameObject != nullptr)
		{
			gameObject->RenderObject();
		}
	}
}

void World::Init()
{
	m_RootSceneObject = new SceneObject(m_gameContext, this);
}
