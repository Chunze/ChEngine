#pragma once

#include "ForceGenerator.h"
#include "glm.h"

class CollisionSpringFG : public ForceGenerator
{
public:
	CollisionSpringFG();

	CollisionSpringFG(Particle* _anchorPoint, glm::vec3 _direction, float _sprintConstant = 10, float _damping = 0.0f);

	~CollisionSpringFG() override;

	virtual void UpdateForce(Particle* particle, float duration);

	/** Anchor point **/
	Particle* Anchor;

	/** Holds the spring constant **/
	float springConstant;

	/** Holds the rest length of the spring. */
	float restLength = 0.0f;

	float dampingConstant;

	// indicates contact face outward direction
	glm::vec3 outwardDirection;

	bool bValid = true;
};