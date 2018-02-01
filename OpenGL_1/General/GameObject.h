#pragma once
#include "World.h"
#include "BaseClass.h"
#include "DrawList.h"

class GameObject : BaseClass
{
public:
	GameObject(GameContext gameContext, World* world);
	World* m_world;

	virtual void CreateDrawListElement(int Mode) = 0;

	World* GetWorld() { return m_world; }

protected:
	inline void AddElementToDrawList(DrawListElement e) { m_gameContext.GetDrawList()->Add(e); }
};