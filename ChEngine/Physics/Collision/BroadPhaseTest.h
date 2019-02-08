#ifndef BROAD_PHASE_TEST_H
#define BROAD_PHASE_TEST_H

class CollisionDetection;

class BroadPhaseTest
{
public:
	BroadPhaseTest(CollisionDetection* collisionDetection);

	CollisionDetection* m_CollisionDetection;
	virtual void RunTest();
};

#endif