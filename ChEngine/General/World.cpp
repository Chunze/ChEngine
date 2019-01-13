#include "World.h"
#include "GameObject.h"

void World::RenderWorld()
{
	for (GameObject* gameObject : m_gameObjects)
	{
		gameObject->RenderObject();
	}
}