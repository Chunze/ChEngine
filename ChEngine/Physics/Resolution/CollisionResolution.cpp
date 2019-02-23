#include "CollisionResolution.h"
#include "MathStatics.h"
#include "PhysicsManager.h"

CollisionResolution::CollisionResolution(PhysicsManager *physicsManager)
	: m_PhysicsManager(physicsManager)
{

}

void CollisionResolution::RunCollisionResolution()
{
	while (!m_PhysicsManager->Collisions.empty())
	{
		CollisionInfo collision = m_PhysicsManager->Collisions.front();
		m_PhysicsManager->Collisions.pop();
		if (collision.m_Contacts.size() == 0)
		{
			continue;
		}
		/** Step 1: construct the contact coordinate axis **/

		// TODO: assume there is only one contact point for now
		vec3 x = collision.m_Contacts[0].m_ContactNormal;
		vec3 y(0.0f, 1.0f, 0.0f);
		vec3 z;

		if (!Math::MakeOrthonormalBasis(x, y, z))
		{
			continue;
		}

		/** Step 2: work out the change in velocity of the contact points **/

		/** Step 3: calculate the impulse needed to result the change in step 2 **/

		/** step 4: split the impulse into linear and angular components **/

	}
}
