#ifndef COLLISION_RESOLUTION_H
#define COLLISION_RESOLUTION_H

class PhysicsManager;

class CollisionResolution
{
public:
	CollisionResolution(PhysicsManager *physicsManager);
	void RunCollisionResolution();

protected:

	PhysicsManager *m_PhysicsManager;
};

#endif // !COLLISION_RESOLUTION_H
