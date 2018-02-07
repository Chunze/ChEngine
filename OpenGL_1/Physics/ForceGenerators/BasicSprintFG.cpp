#include "BasicSprintFG.h"
#include "glm.h"
#include "Particle.h"

BasicSprintFG::BasicSprintFG()
{

}

BasicSprintFG::BasicSprintFG(Particle* _other, float _restLength, float _sprintConstant /*= 10*/, float _damping /*= 0*/)
	: other(_other), restLength(_restLength), springConstant(_sprintConstant), dampingConstant(_damping)
{}

void BasicSprintFG::UpdateForce(Particle* particle, float duration)
{
	glm::vec3 springForceDir, springForce, dampingForce;

	// pointing from this particle to other
	springForceDir = other->m_position - particle->m_position;
	float distance = glm::length(springForce);
	springForceDir = glm::normalize(springForceDir);

	springForce = -springForceDir * springConstant * (distance - restLength);
	dampingForce = -dampingConstant * ((particle->m_volecity - other->m_volecity) * springForceDir) * springForceDir;

	glm::vec3 force = springForce + dampingForce;

	particle->addForce(force);
}
