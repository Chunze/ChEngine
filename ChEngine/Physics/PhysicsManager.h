#ifndef PHYSICS_MANAGER_H
#define PHYSICS_MANAGER_H
#include <vector>

#include "PhysicsTypes.h"
#include "CollisionDetection.h"
#include "CollisionResolution.h"

#include "BaseClass.h"
#include "ParticleForceRegistry.h"
#include "glm.h"

class ForceGenerator;
class ParticleContact;
class ParticleContactResolver;
class ParticleContactGenerator;

using namespace std;

class PhysicsManager : public BaseClass
{
	friend class CollisionDetection;
	friend class CollisionResolution;
public:

	PhysicsManager(GameContext* gameContext);

	ParticleForceRegistry m_particleForceRegistry;

	ParticleForceRegistry m_contactForceRegistry;

	void registerForce(Particle* particle, ForceGenerator* FG);

	void Update(float Delta);

	void Integrate(float Delta);

	// updating constant forces
	void UpdateForces(float Delta);

	// updating forces for one frame, new contact forces will be generated next frame
	void UpdateContactForces(float Delta);

	void ConstructPhysicsScene(PhysicsProxies Proxies);

	void GenerateCollisionInfo(Particle* particle, Particle* Anchor, glm::vec3 OutwardDirection, float _springConstant, float _damping);

	void AddPhysicsParticle(Particle_sp ParticleToAdd);
	void AddParticleContactGenerator(ParticleContactGenerator* contactGenerator);
	void RegisterPhysicsBody(RigidBody_sp BodyToAdd);
	void RegisterCollisionPrimitive(CollisionPrimitive_sp PrimitiveToAdd);

	void Increment_RK4_step() { RK4_step++; }
	void Reset_RK4_Step() { RK4_step = 1; }
	
	/**    getters    **/
	int GetIntegrator() { return m_intergrator; }
	int GetRK4StepCount() { return RK4_step; }
	float GetLinearDamping() { return m_LinearDamping; }
	float GetAngularDamping() { return m_AngularDamping; }
	glm::vec3 GetGravity() { return m_Gravity; }
	RigidBodies* GetRigidBodies() { return &m_RigidBodies; }

	/**    setters    **/
	void SetIntegrator(int in) { m_intergrator = in; }

protected:
	void Init();

	// 0 = Euler; 1 = RK4
	int m_intergrator;

	// RK4 step count
	int RK4_step = 1;
	Particles m_physicsParticles;
	RigidBodies m_RigidBodies;
	ParticleContacts m_ParticleContacts;
	CollisionInfo m_Collisions;
	std::vector<ParticleContactGenerator*> m_ContactGenerator;
	std::unique_ptr<ParticleContactResolver> m_ParticleContactResolver = nullptr;

	// The height of a simple plane for testing (upward normal)
	float m_PlaneHeight = 0.0f;

	// physics constants
	glm::vec3 m_Gravity = glm::vec3(0.0f, -9.8f, 0.0f);
	float m_LinearDamping;
	float m_AngularDamping;
	float m_LinearDampingCoef = 0.93f;
	float m_AngularDampingCoef = 0.96f;

	// worker classes
	CollisionDetection m_CollisionDetection;
	CollisionResolution m_CollisionResolution;

	void RunCollisionDetection();
	void RunCollisionResolution(float Delta);
};
#endif