#pragma once
#include <vector>
#include "SceneObject.h"
#include "DrawList.h"
#include "Component.h"

class World;

class GameObject : public SceneObject
{
	typedef SceneObject Super;
public:
	GameObject(GameContext* gameContext, World* world);

	GameObject(GameContext* gameContext, World* world, glm::vec3 location);

	virtual void RenderObject();

	virtual void Update(float Delta) override;

	void AddComponent(Component* ComponentToAdd);

protected:
	

	Shader* shader;

	DrawListElement e;

	std::vector<Component*> m_Components;

	bool bUpdateEnabled = true;

	virtual void CreateAndAddDrawListElement(int Mode);

	//inline int AddElementToDrawList(DrawListElement e, int index) { return m_gameContext->GetDrawList()->AddToDrawQ(e, index); }
};