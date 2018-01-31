#pragma once
#include <glm/glm.hpp>

class Particle
{
public:
	Particle();

	Particle(float mass, glm::vec3 position, glm::vec3 volecity);

	float m_inverseMass;
	glm::vec3 m_position;
	glm::vec3 m_volecity;

	glm::vec3 m_gravity = glm::vec3(0.0f, -9.8f, 0.0f);

	/**
	* Adds the given force to the particle to be applied at the
	* next iteration only.
	*/
	void addForce(const glm::vec3 &force);

protected:
	/**
	* Holds the accumulated force to be applied at the next
	* simulation iteration only. This value is zeroed at each
	* integration step.
	*/
	glm::vec3 forceAccum;
};