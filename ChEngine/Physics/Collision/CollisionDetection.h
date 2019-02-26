#ifndef COLLISION_DETECTION_H
#define COLLISION_DETECTION_H

#include "BroadPhaseTest.h"
#include "NarrowPhaseTest.h"
#include "PhysicsTypes.h"

class PhysicsManager;

class CollisionDetection
{
	friend class BroadPhaseTest;
	friend class NarrowPhaseTest;

public:
	CollisionDetection(PhysicsManager* PhysicsManager);

	void RunCollisionDetection();

	void ReportCollision(CollisionInfo &Collision) const;

	void RegisterCollisionPrimitive(CollisionPrimitive_sp PrimitiveToAdd);

	CollisionPrimitives *GetRegisteredPrimitives() { return &m_PrimitiveWithoutBody; }

protected:
	PhysicsManager* m_PhysicsManager;
	BroadPhaseTest m_BroadPhaseTest;
	NarrowPhaseTest m_NarrowPhaseTest;


	CollisionPrimitives m_PrimitiveWithoutBody;

	/// Potential body contacts.
	/// Broad Phase Test will write to this.
	/// Narrow Phase Test will read from this.
	PotentialContacts m_PotentialContacts;

	void RunBroadPhase();
	void RunNarrowPhase();
};

#endif
