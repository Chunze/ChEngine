#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <vector>
#include "SceneObject.h"
#include "DrawList.h"
#include "Component.h"

class World;

class GameObject : public SceneObject
{
	typedef SceneObject Super;
public:
	GameObject(GameContext* gameContext, World* world, glm::vec3 location = vec3(), quat rotation = quat());

	virtual void Update(float Delta) override;

	virtual void UpdateTransform() override;

	virtual void PostPhysicsUpdate();

	void AddComponent(Component_sp ComponentToAdd);
	void SetRootComponent(Component_sp Root);

	Component_sp GetRootComponent() { return m_RootComponent; }

protected:
	Component_sp m_RootComponent;

	Shader* shader;

	DrawListElement e;

	std::vector<Component_sp> m_Components;

	bool bUpdateEnabled = true;
};

#endif // GAME_OBJECT_H
