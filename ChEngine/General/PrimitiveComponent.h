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
	DrawListElement m_DrawListElement;
};

