#pragma once
#include "BaseClass.h"
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
	void UpdateTransform();

	void AddChild(SceneObject* child) { m_Children.push_back(child); }
	void RemoveChild(SceneObject* child);

	/**    setters    **/
	void SetWorldTransform(glm::mat4 worldTransform) { m_WorldTransform = worldTransform; }
	void SetWorldLocation(glm::vec3 location);
	void SetWorld(World* world) { m_World = world; }
	void SetOwner(SceneObject* owner);

	/**    getters    **/
	glm::mat4 GetWorldTransform() { return m_WorldTransform; }
	glm::vec3 GetWorldLocation() { return glm::vec3(m_WorldTransform[3]); }
	World* GetWorld() { return m_World; }
	SceneObject* GetOwner() { return m_Owner; }
	std::vector<SceneObject*> &GetChildren() { return m_Children; }

	~SceneObject();

protected:
	glm::mat4 m_RaletiveTransform = glm::mat4();

	glm::mat4 m_WorldTransform = glm::mat4();
	
	World* m_World;

	SceneObject* m_Owner = nullptr;
	std::vector<SceneObject*> m_Children;
};

