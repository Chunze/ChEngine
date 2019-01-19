#include "SceneObject.h"



SceneObject::SceneObject()
{
}

SceneObject::SceneObject(GameContext* gameContext)
	: Super(gameContext)
{

}

SceneObject::SceneObject(GameContext* gameContext, World* world)
	: Super(gameContext),
	m_World(world)
{

}

SceneObject::SceneObject(GameContext* gameContext, World* world, glm::vec3 location)
	: SceneObject(gameContext, world)
{
	SetWorldLocation(location);
}

void SceneObject::Update(float Delta)
{
	for (SceneObject *sceneObject : m_Children)
	{
		sceneObject->Update(Delta);
	}

	bPostPhysicsUpdated = false;
}

void SceneObject::UpdateTransform()
{
	if (bPostPhysicsUpdated)
	{
		return;
	}

	SetRaletiveLocation(m_Particle->GetPosition());
	if (m_Owner != nullptr)
	{
		if (!m_Owner->HasPostPhysicsUpdated())
		{
			m_Owner->UpdateTransform();
		}
		m_WorldTransform = m_Owner->GetWorldTransform() * m_RaletiveTransform;
	}
	else
	{
		m_WorldTransform = m_RaletiveTransform;
	}
}

void SceneObject::PostPhysicsUpdate()
{
	if (bPostPhysicsUpdated)
	{
		return;
	}

	if (m_Particle != nullptr)
	{
		if (bIsRoot)
		{
			SetWorldLocation(m_Particle->GetPosition());
			return;
		}
		else
		{
			SetRaletiveLocation(m_Particle->GetPosition());
		}
	}

	if (m_Owner != nullptr)
	{
		if (!m_Owner->HasPostPhysicsUpdated())
		{
			m_Owner->UpdateTransform();
		}
		m_WorldTransform = m_Owner->GetWorldTransform() * m_RaletiveTransform;
	}
	else
	{
		m_WorldTransform = m_RaletiveTransform;
	}
}

void SceneObject::SetWorldTransform(glm::mat4 worldTransform)
{ 
	m_WorldTransform = worldTransform;
	if (m_Particle != nullptr)
	{
		m_Particle->SetPosition(glm::vec3(m_WorldTransform[3]));
	}
}

void SceneObject::RemoveChild(SceneObject* child)
{
// 	for (size_t index = 0; index < m_Children.size(); index++)
// 	{
// 		if (m_Children[index] == child)
// 		{
// 			m_Children.erase(m_Children.begin() + index);
// 		}
// 	}
}

void SceneObject::SetWorldLocation(glm::vec3 location)
{
	m_WorldTransform[3] = glm::vec4(location, 1.0f);
}

void SceneObject::SetOwner(SceneObject* owner)
{
	if (m_Owner == nullptr)
	{
		m_Owner = owner;
	}
}

SceneObject::~SceneObject()
{
}
