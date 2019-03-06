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

	void ResolvePenetrations();

	void ResolveVelocities();
};

#endif // !COLLISION_RESOLUTION_H
