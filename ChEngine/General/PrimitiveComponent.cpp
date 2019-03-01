#include "PrimitiveComponent.h"
#include "RigidBody.h"



PrimitiveComponent::PrimitiveComponent(GameContext* gameContext, World* world)
	: Super(gameContext, world)
{
}

void PrimitiveComponent::PostPhysicsUpdate()
{
	if (m_RigidBody != nullptr)
	{
		if (bIsRoot)
		{
			SetWorldTransform(m_RigidBody->GetTransform());
		}
		else
		{
			SetRelativeTransform(m_RigidBody->GetTransform());
		}
	}
}

void PrimitiveComponent::SetWorldTransform(mat4 worldTransform)
{
	Super::SetWorldTransform(worldTransform);

	m_RigidBody->SetTransform(worldTransform);
}

void PrimitiveComponent::SetWorldLocation(vec3 location)
{
	Super::SetWorldLocation(location);
	m_RigidBody->m_Transform[3] = vec4(location, 1.0f);
}

PrimitiveComponent::~PrimitiveComponent()
{
}
