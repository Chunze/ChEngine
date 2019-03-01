#pragma once
#include "BaseClass.h"
#include "Particle.h"

#include "PhysicsTypes.h"

#include <vector>

using namespace std;

class SceneObject : public BaseClass
{
	typedef BaseClass Super;

public:
	SceneObject();
	SceneObject(GameContext* gameContext);
	SceneObject(GameContext* gameContext, World* world, glm::vec3 location = vec3(), quat rotation = quat());

	virtual void Update(float Delta);
	virtual void UpdateTransform();
	//virtual void PostPhysicsUpdate();

	/**    setters    **/
	void SetRelativeLocation(glm::vec3 location) { m_RelativeTransform[3] = glm::vec4(location, 1.0); }
	void SetWorldTransform(const vec3 &location, const quat &orientation);
	virtual void SetWorldTransform(glm::mat4 worldTransform);
	virtual void SetRelativeTransform(mat4 relativeTransform);
	virtual void SetWorldLocation(glm::vec3 location);
	void SetWorld(World* world) { m_World = world; }

	/**    getters    **/
	glm::mat4 GetWorldTransform() { return m_WorldTransform; }
	glm::vec3 GetWorldLocation() { return glm::vec3(m_WorldTransform[3]); }
	World* GetWorld() { return m_World; }
	bool HasPostPhysicsUpdated() { return bPostPhysicsUpdated; }
	Particle_sp GetParticle() { return m_Particle; }

	~SceneObject();

protected:
	glm::mat4 m_RelativeTransform = glm::mat4();

	glm::mat4 m_WorldTransform = glm::mat4();
	
	World* m_World;
	Particle_sp m_Particle = nullptr;
	bool bPostPhysicsUpdated = false;


};

