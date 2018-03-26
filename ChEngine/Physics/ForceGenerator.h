#ifndef FORCE_GENERATOR_H
#define FORCE_GENERATOR_H

#include "Particle.h"

class ForceGenerator
{
public:
	/**
	* Overload this in implementations of the interface to calculate
	* and update the force applied to the given particle.
	*/
	virtual void UpdateForce(Particle* particle, float duration) = 0;

	virtual void Destroy() {};

	virtual ~ForceGenerator() = 0
	{

	}
};

#endif