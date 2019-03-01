#include "SceneObject.h"
#include "MathStatics.h"


SceneObject::SceneObject()
{
}

SceneObject::SceneObject(GameContext* gameContext)
	: Super(gameContext)
{

}

SceneObject::SceneObject(GameContext* gameContext, World* world, glm::vec3 location, quat rotation)
	: Super(gameContext),
	  m_World(world)
{
	SetWorldTransform(location, rotation);
}

void SceneObject::Update(float Delta)
{

	bPostPhysicsUpdated = false;
}

void SceneObject::UpdateTransform()
{

}

void SceneObject::SetWorldTransform(glm::mat4 worldTransform)
{ 
	m_WorldTransform = worldTransform;
	if (m_Particle != nullptr)
	{
		m_Particle->SetPosition(glm::vec3(m_WorldTransform[3]));
	}
}

void SceneObject::SetWorldTransform(const vec3 &location, const quat &orientation)
{
	SetWorldTransform(Math::MakeTransform(location, orientation));
}

void SceneObject::SetRelativeTransform(mat4 relativeTransform)
{
	m_RelativeTransform = relativeTransform;
}

void SceneObject::SetWorldLocation(glm::vec3 location)
{
	m_WorldTransform[3] = glm::vec4(location, 1.0f);
	if (m_Particle != nullptr)
	{
		m_Particle->SetPosition(location);
	}
}

SceneObject::~SceneObject()
{
}
