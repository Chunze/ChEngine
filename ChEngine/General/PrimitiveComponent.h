#pragma once
#include "Component.h"
#include "DrawList.h"

class PrimitiveComponent : public Component
{
public:
	PrimitiveComponent(GameContext* gameContext);

	void AddDrawListElement() override;

	~PrimitiveComponent();

protected:
	bool bDirty = true;
	DrawListElement m_DrawListElement;
};

