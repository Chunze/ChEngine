#include "World.h"
#include "GameObject.h"

void World::Update(float Delta)
{
	for (GameObject* gameObject : m_gameObjects)
	{
		gameObject->Update(Delta);
	}
}

void World::RenderWorld()
{
	for (GameObject* gameObject : m_gameObjects)
	{
		gameObject->RenderObject();
	}
}