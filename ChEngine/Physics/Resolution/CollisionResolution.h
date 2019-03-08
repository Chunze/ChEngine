#ifndef COLLISION_RESOLUTION_H
#define COLLISION_RESOLUTION_H

class PhysicsManager;

class CollisionResolution
{
public:
	CollisionResolution(PhysicsManager *physicsManager);
	void RunCollisionResolution(float duration);

protected:

	PhysicsManager *m_PhysicsManager;
	size_t m_NumberContacts;
	unsigned m_MaxIteration = 0;

	void PrepareContacts();

	void ResolvePenetrations();

	void ResolveVelocities(float duration);

	void SetMaxIteration(size_t ContactNum, unsigned Multiplier);
};

#endif // !COLLISION_RESOLUTION_H
