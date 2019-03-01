#pragma once
#include "Component.h"
#include "DrawList.h"
#include "IPhysicsProxy.h"

class PrimitiveComponent : public Component, public IPhysicsProxy
{
	typedef Component Super;
public:
	PrimitiveComponent(GameContext* gameContext, World* world);

	void SetRigidBody(RigidBody_sp RigidBody) { m_RigidBody = RigidBody; }

	/** IPhysicsProxy functions **/
	virtual RigidBody_sp GetRigidBody() const { return m_RigidBody; }
	virtual CollisionPrimitive_sp GetCollisionPrimitive() const { return nullptr; }
	virtual void PostPhysicsUpdate();
	/** IPhysicsProxy functions **/

	void SetWorldTransform(mat4 worldTransform) override;
	void SetWorldLocation(vec3 location) override;

	~PrimitiveComponent();

protected:
	bool bDirty = true;
	DrawListElement m_DrawListElement;

	RigidBody_sp m_RigidBody;
};

