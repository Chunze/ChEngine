#pragma once
#include <vector>
#include "World.h"
#include "BaseClass.h"
#include "DrawList.h"
#include "Component.h"

class GameObject : public BaseClass
{
public:
	GameObject(GameContext* gameContext, World* world);

	GameObject(GameContext* gameContext, World* world, glm::vec3 location);

	virtual void RenderObject();

	virtual void Update(float Delta) {}

	World* GetWorld() { return m_world; }

	glm::mat4 GetWorldTransform() { return m_WorldTransform; }

	void SetLocation(glm::vec3 NewLocation);

protected:
	World* m_world;

	Shader* shader;

	DrawListElement e;

	glm::mat4 m_WorldTransform = glm::mat4();

	std::vector<Component*> m_Components;

	bool bUpdateEnabled = true;

	virtual void CreateAndAddDrawListElement(int Mode);

	//inline int AddElementToDrawList(DrawListElement e, int index) { return m_gameContext->GetDrawList()->AddToDrawQ(e, index); }
};