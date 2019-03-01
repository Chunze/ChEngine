#ifndef PHYSICS_PROXY_H
#define PHYSICS_PROXY_H
#pragma once

#include "PhysicsTypes.h"

class RigidBody;

class IPhysicsProxy
{
public:
	virtual RigidBody_sp GetRigidBody() const = 0;
	virtual CollisionPrimitive_sp GetCollisionPrimitive() const = 0;

	virtual void PostPhysicsUpdate() = 0;
};

#endif
