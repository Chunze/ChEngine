#pragma once
#include "Component.h"
#include "DrawList.h"

class PrimitiveComponent : public Component
{
	typedef Component Super;
public:
	PrimitiveComponent(GameContext* gameContext, World* world);

	void AddDrawListElement() override;

	void PostPhysicsUpdate() override;

	void SetRigidBody(RigidBody_sp RigidBody) { m_RigidBody = RigidBody; }

	RigidBody_sp GetPhsicsBody() { return m_RigidBody; }

	void SetWorldTransform(mat4 worldTransform) override;
	void SetWorldLocation(vec3 location) override;

	~PrimitiveComponent();

protected:
	bool bDirty = true;
	DrawListElement m_DrawListElement;

	RigidBody_sp m_RigidBody;
};

