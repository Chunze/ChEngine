#include "World.h"
#include "GameObject.h"
#include "IPhysicsProxy.h"
#include "IRenderableObject.h"
#include "SceneObject.h"

World::World(GameContext* gameContext)
	: BaseClass(gameContext)
{
}

void World::Update(float Delta)
{
	for (Component_sp Component : m_Components)
	{
		Component->Update(Delta);
	}
	for (auto gameObject : m_GameObjects)
	{
		gameObject->Update(Delta);
	}
}

void World::PostPhysicsUpdate()
{
	for (Component_sp component : m_Components)
	{
		// Check if the component is IPhysicsProxy, call post physics update if yes.
		IPhysicsProxy* Proxy = dynamic_cast<IPhysicsProxy*>(component.get());
		if (Proxy)
		{
			Proxy->PostPhysicsUpdate();
		}
	}

	for (auto gameObject : m_GameObjects)
	{
		gameObject->PostPhysicsUpdate();
	}
}

void World::RegisterGameObject(GameObject_sp Object)
{
	for (auto o : m_GameObjects)
	{
		if (o == Object)
		{
			return;
		}
	}

	m_GameObjects.push_back(Object);
}

void World::RegisterComponent(Component_sp Component)
{
	for (auto c : m_Components)
	{
		if (c == Component)
		{
			return;
		}
	}

	m_Components.push_back(Component);
}

PhysicsProxies World::GetPhysicsProxies()
{
	PhysicsProxies Proxies;
	for (Component_sp component : m_Components)
	{
		IPhysicsProxy* Proxy = dynamic_cast<IPhysicsProxy*>(component.get());
		if (Proxy)
		{
			Proxies.push_back(Proxy);
		}
	}

	return Proxies;
}

RenderableObjects World::GetRenderableObjects()
{
	RenderableObjects Objects;
	for (Component_sp component : m_Components)
	{
		IRenderableObject* Proxy = dynamic_cast<IRenderableObject*>(component.get());
		if (Proxy)
		{
			Objects.push_back(Proxy);
		}
	}

	return Objects;
}

void World::Init()
{
	//m_RootSceneObject = new SceneObject(m_gameContext, this);
}
