#ifndef BASIC_SPRINT_FORCE_GENERATOR_H
#define BASIC_SPRINT_FORCE_GENERATOR_H
#pragma once

#include "ForceGenerator.h"

class Particle;

class BasicSprintFG : public ForceGenerator
{

public:
	BasicSprintFG();

	~BasicSprintFG() override { delete other; }

	BasicSprintFG(Particle* _other, float _restLength, float _sprintConstant = 10, float _damping = 0.0f);

	virtual void UpdateForce(Particle* particle, float duration);

	virtual void Destroy() override;

	/** The particle at the other end of the spring **/
	Particle *other;

	/** Holds the spring constant **/
	float springConstant;

	/** Holds the rest length of the spring. */
	float restLength;

	float dampingConstant;
};

#endif
