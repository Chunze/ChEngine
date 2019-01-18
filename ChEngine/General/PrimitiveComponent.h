#pragma once
#include "Component.h"
#include "DrawList.h"

class PrimitiveComponent : public Component
{
	typedef Component Super;
public:
	PrimitiveComponent(GameContext* gameContext, World* world);

	void AddDrawListElement() override;

	~PrimitiveComponent();

protected:
	bool bDirty = true;
	DrawListElement m_DrawListElement;
};

