#ifndef COLLISION_DETECTION_H
#define COLLISION_DETECTION_H
#include <vector>

#include "CollisionTestSelector.h"
#include "PhysicsTypes.h"

class PhysicsManager;

class CollisionDetection
{
public:

	CollisionTestSelector m_CollisionTestSelector;

	void RunCollisionDetection();

	void RegisterCollisionPrimitive(CollisionPrimitive_sp PrimitiveToAdd);

protected:
	PhysicsManager* m_PhysicsManager;

	CollisionPrimitives m_CollisionPrimitives;

	void RunBroadPhase();
	void RunNarrowPhase();
};

#endif
