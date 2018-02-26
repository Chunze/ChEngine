#include "CollisionSpringFG.h"

CollisionSpringFG::CollisionSpringFG()
{

}

CollisionSpringFG::CollisionSpringFG(Particle* _anchorPoint, glm::vec3 _direction, float _sprintConstant /*= 10*/, float _damping /*= 0.0f*/)
	: Anchor(_anchorPoint),
	  outwardDirection(_direction),
	  springConstant(_sprintConstant),
	  dampingConstant(_damping)
{

}

void CollisionSpringFG::UpdateForce(Particle* particle, float duration)
{
	glm::vec3 springForceDir, springForce, dampingForce;

	// pointing from this particle to other
	springForceDir = particle->m_position - Anchor->m_position;

	if (glm::dot(springForceDir, outwardDirection) < 0 || !bValid)
	{
		// the particle is no longer outside of the contact face
		bValid = false;
		return;
	}

	float distance = glm::length(springForceDir);
	springForceDir = springForceDir / distance;

	springForce = -springForceDir * springConstant * (distance - restLength);
	dampingForce = -springForceDir * dampingConstant * ((particle->m_volecity - Anchor->m_volecity) * springForceDir);

	glm::vec3 force = springForce + dampingForce;

	if (glm::length(force) > 1e10)
	{
		k1 += 1.0f;
	}

	particle->addForce(force);
}
