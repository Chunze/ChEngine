#ifndef NARROW_PHASE_TEST_H
#define NARROW_PHASE_TEST_H
#include "CollisionTestSelector.h"

class CollisionDetection;

class NarrowPhaseTest
{
public:
	NarrowPhaseTest(CollisionDetection* collisionDetection);

	CollisionDetection* m_CollisionDetection;
	virtual void RunTest(CollisionInfo *Data);

protected:
	/// selects narrow phase collision tests, i.e. SphereVsSphere.
	CollisionTestSelector m_CollisionTestSelector;
};

#endif
