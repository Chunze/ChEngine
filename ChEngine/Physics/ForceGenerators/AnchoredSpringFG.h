#pragma once
#include "ForceGenerator.h"
class AnchoredSpringFG : public ForceGenerator
{
public:
	AnchoredSpringFG();

	AnchoredSpringFG(glm::vec3 AnchorPoint, float SprintConstant, float RestLength);

	virtual void UpdateForce(Particle* particle, float duration) override;

	~AnchoredSpringFG();

protected:

	glm::vec3 m_AnchorPoint;
	float m_SprintConstant;
	float m_RestLength;
};

