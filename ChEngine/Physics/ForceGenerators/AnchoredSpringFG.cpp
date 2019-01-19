#include "AnchoredSpringFG.h"



AnchoredSpringFG::AnchoredSpringFG()
{
}


AnchoredSpringFG::AnchoredSpringFG(glm::vec3 AnchorPoint, float SprintConstant, float RestLength)
	: m_AnchorPoint(AnchorPoint)
	, m_SprintConstant(SprintConstant)
	, m_RestLength(RestLength)
{

}

void AnchoredSpringFG::UpdateForce(Particle* particle, float duration)
{
	glm::vec3 AnchorToParticle = particle->GetPosition() - m_AnchorPoint;
	float SprintLength = glm::length(AnchorToParticle);
	float ForceMagnitude = (m_RestLength - SprintLength) * m_SprintConstant;
	glm::vec3 Force = glm::normalize(AnchorToParticle) * ForceMagnitude;
	particle->addForce(Force);
}

AnchoredSpringFG::~AnchoredSpringFG()
{
}
