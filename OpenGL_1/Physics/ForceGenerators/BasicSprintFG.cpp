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
	float dragCoefficient = magnitude;
	magnitude = fabsf(magnitude - restLength);
	magnitude *= springConstant;

	force = glm::normalize(force);
	force *= -magnitude;

	//dragCoefficient = k1 * dragCoefficient + k2 * dragCoefficient * dragCoefficient;

	particle->addForce(force);
}
