#ifndef PHYSICS_STATICS_H
#define PHYSICS_STATICS_H

#include <memory>
#include "glm.h"
#include "PhysicsManager.h"

namespace ChEngine
{
	namespace PhysicsStatics
	{
		static std::shared_ptr<RigidSphere> NewRigidSphere(PhysicsManager* Manager, float Radius)
		{
			auto Object = std::make_shared<RigidSphere>(Manager, Radius);

			Manager->RegisterPhysicsBody(Object);
			Object->RegisterDefaultCollision();

			return Object;
		}

		static std::shared_ptr<RigidBox> NewRigidBox(PhysicsManager* Manager, vec3 HalfSize)
		{
			auto Object = std::make_shared<RigidBox>(Manager, HalfSize);

			Manager->RegisterPhysicsBody(Object);
			Object->RegisterDefaultCollision();

			return Object;
		}
	}
}
#endif // !PHYSICS_STATICS_H
