#pragma once
#include "ForceGenerator.h"


class FakeSprintFG : public ForceGenerator
{
public:
	FakeSprintFG();
	FakeSprintFG(glm::vec3 AnchorPoint, float SpringConstant, float Damping);

	virtual void UpdateForce(Particle* particle, float duration) override;

	~FakeSprintFG();

protected:

	glm::vec3 m_AnchorPoint;
	float m_SpringConstant;
	float m_Damping;
};
