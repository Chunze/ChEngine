#pragma once
#include "BaseClass.h"
#include "Particle.h"
#include <vector>

class SceneObject : public BaseClass
{
	typedef BaseClass Super;

public:
	SceneObject();
	SceneObject(GameContext* gameContext);
	SceneObject(GameContext* gameContext, World* world);
	SceneObject(GameContext* gameContext, World* world, glm::vec3 location);

	virtual void Update(float Delta);
	virtual void UpdateTransform();
	virtual void PostPhysicsUpdate();

	void AddChild(SceneObject* child) { m_Children.push_back(child); }
	void RemoveChild(SceneObject* child);

	/**    setters    **/
	void SetRaletiveLocation(glm::vec3 location) { m_RaletiveTransform[3] = glm::vec4(location, 1.0); }
	void SetWorldTransform(glm::mat4 worldTransform);
	void SetWorldLocation(glm::vec3 location);
	void SetWorld(World* world) { m_World = world; }
	void SetOwner(SceneObject* owner);
	void SetIsRoot(bool IsRoot) { bIsRoot = IsRoot; }

	/**    getters    **/
	glm::mat4 GetWorldTransform() { return m_WorldTransform; }
	glm::vec3 GetWorldLocation() { return glm::vec3(m_WorldTransform[3]); }
	World* GetWorld() { return m_World; }
	SceneObject* GetOwner() { return m_Owner; }
	std::vector<SceneObject*> &GetChildren() { return m_Children; }
	bool HasPostPhysicsUpdated() { return bPostPhysicsUpdated; }
	bool GetIsRoot() { return bIsRoot; }
	Particle* GetParticle() { return m_Particle; }

	~SceneObject();

protected:
	glm::mat4 m_RaletiveTransform = glm::mat4();

	glm::mat4 m_WorldTransform = glm::mat4();
	
	World* m_World;
	Particle* m_Particle = nullptr;

	SceneObject* m_Owner = nullptr;
	std::vector<SceneObject*> m_Children;

	bool bPostPhysicsUpdated = false;

	// This indicates whether this SceneObject is the root of some GameObject
	bool bIsRoot = false;
};

