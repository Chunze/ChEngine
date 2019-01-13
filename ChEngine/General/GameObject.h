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
	World* m_world;

	virtual void CreateAndAddDrawListElement(int Mode);

	virtual void Update(float Delta) {}

	World* GetWorld() { return m_world; }

protected:
	Shader* shader;

	DrawListElement e;

	std::vector<Component*> m_Components;

	bool bUpdateEnabled = true;

	//inline int AddElementToDrawList(DrawListElement e, int index) { return m_gameContext->GetDrawList()->AddToDrawQ(e, index); }
};