#pragma once
#include <glm/glm.hpp>

class Particle
{
public:
	Particle();

	Particle(float mass, glm::vec3 position, glm::vec3 volecity);

	float m_inverseMass;
	glm::vec3 m_position = glm::vec3(0.0f);
	glm::vec3 m_volecity = glm::vec3(0.0f);
	glm::vec3 m_acceleration = glm::vec3(0.0f);

	// used for RK4 calculation
	glm::vec3 m_prePosition = glm::vec3(0.0f);
	glm::vec3 m_preVolecity = glm::vec3(0.0f);

	glm::vec3 m_gravity = glm::vec3(0.0f, -9.8f, 0.0f);

	float damping = 0.98f;

	/**
	* Adds the given force to the particle to be applied at the
	* next iteration only.
	*/
	void addForce(const glm::vec3 &force);

	void Integrate_Euler(float Delta);

	void Integrate_Rk4(float Delta, int step);



	void ClearForce();
protected:
	/**
	* Holds the accumulated force to be applied at the next
	* simulation iteration only. This value is zeroed at each
	* integration step.
	*/
	glm::vec3 m_forceAccum = glm::vec3(0.0f);

	bool bUseGravite = true;

private:
	// Intermediate value for RK4 calculation
	glm::vec3 RK4_V_1, RK4_V_2, RK4_V_3, RK4_V_4;
	glm::vec3 RK4_A_1, RK4_A_2, RK4_A_3, RK4_A_4;
};