#pragma once
#include "World.h"
#include "BaseClass.h"
#include "DrawList.h"

class GameObject : public BaseClass
{
public:
	GameObject(GameContext gameContext, World* world);
	World* m_world;

	virtual void CreateAndAddDrawListElement(int Mode) = 0;

	virtual void Update(float Delta) {}

	World* GetWorld() { return m_world; }

protected:
	Shader* shader;

	//inline int AddElementToDrawList(DrawListElement e, int index) { return m_gameContext.GetDrawList()->AddToDrawQ(e, index); }
};