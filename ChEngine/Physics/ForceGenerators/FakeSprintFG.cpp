#include "FakeSprintFG.h"



FakeSprintFG::FakeSprintFG()
{
}


FakeSprintFG::FakeSprintFG(glm::vec3 AnchorPoint, float SpringConstant, float Damping)
	: m_AnchorPoint(AnchorPoint), m_SpringConstant(SpringConstant), m_Damping(Damping)
{

}

void FakeSprintFG::UpdateForce(Particle* particle, float duration)
{
	// position at the start of the frame
	glm::vec3 StartPosition = particle->GetPosition() - m_AnchorPoint;

	float gamma = 0.5f * sqrtf(4 * m_SpringConstant - m_Damping * m_Damping);
	if (gamma == 0.0f)
	{
		return;
	}

	glm::vec3 c = StartPosition * (0.5f * m_Damping / gamma) + particle->GetVelocity() / gamma;

	// calculate position at the end of the frame
	glm::vec3 EndPosition = StartPosition * cosf(gamma * duration) + c * sinf(gamma * duration);
	EndPosition *= expf(-0.5f * duration * m_Damping);

	// calculate resulting acceleration
	glm::vec3 accel = (EndPosition - StartPosition) * (2 / (duration * duration)) - particle->GetVelocity() / duration;
	accel = 2.0f * ((EndPosition - StartPosition) - particle->GetVelocity() * duration) / (duration * duration);
	particle->addForce(accel * particle->GetMass());
}

FakeSprintFG::~FakeSprintFG()
{
}
