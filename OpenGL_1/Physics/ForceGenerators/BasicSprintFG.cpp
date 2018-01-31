#include "BasicSprintFG.h"
#include "glm.h"
#include "Particle.h"

BasicSprintFG::BasicSprintFG()
{

}

BasicSprintFG::BasicSprintFG(Particle* _other, float _restLength, float _sprintConstant /*= 100*/)
	: other(_other), restLength(_restLength), springConstant(_sprintConstant)
{}

void BasicSprintFG::UpdateForce(Particle* particle, float duration)
{
	glm::vec3 force = particle->m_position - other->m_position;

	float magnitude = glm::length(force);
	magnitude = fabsf(magnitude - restLength);
	magnitude *= springConstant;

	force = glm::normalize(force);
	force *= -magnitude;
	particle->addForce(force);
}
