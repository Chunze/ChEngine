#pragma once
#include "glm.h"

class PhysicsManager;

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

	float m_DampingCoef = 0.0f;

	void AddVelocity(glm::vec3 VelocityToAdd) { m_volecity += VelocityToAdd; }
	void AddPosition(glm::vec3 PositionToAdd) { m_position += PositionToAdd; }

	/**    setters    **/
	void SetPosition(glm::vec3 NewPos) { m_position = NewPos; }
	void SetVelocity(glm::vec3 NewVel) { m_volecity = NewVel; }

	/**    getters    **/
	glm::vec3 GetPosition() { return m_position; }
	glm::vec3 GetVelocity() { return m_volecity; }
	glm::vec3 GetAcceleration() { return m_acceleration; }
	float GetInverseMass() { return m_inverseMass; }
	float GetMass() { return 1.0f / m_inverseMass; }

	/**
	* Adds the given force to the particle to be applied at the
	* next iteration only.
	*/
	void addForce(const glm::vec3 &force);

	void Integrate(float Delta);

	void Integrate_Euler(float Delta);

	void Integrate_Rk4(float Delta, int step);

	void SetPhysicsManager(PhysicsManager* physicsManager) { m_PhysicsManager = physicsManager; }

	void ClearForce();
protected:
	/**
	* Holds the accumulated force to be applied at the next
	* simulation iteration only. This value is zeroed at each
	* integration step.
	*/
	glm::vec3 m_forceAccum = glm::vec3(0.0f);

	bool bUseGravite = true;

	PhysicsManager* m_PhysicsManager;

private:
	// Intermediate value for RK4 calculation
	glm::vec3 RK4_V_1, RK4_V_2, RK4_V_3, RK4_V_4;
	glm::vec3 RK4_A_1, RK4_A_2, RK4_A_3, RK4_A_4;
};