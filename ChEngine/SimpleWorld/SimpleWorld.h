#pragma once
#include "World.h"

class SimpleWorld : public World
{
public:
	SimpleWorld(GameContext* gameContext);
	~SimpleWorld();

	void Update(float Delta) override;
	void InitWorld();
	virtual void InitCamera() override;

	void InitLight();
	void InitDebugElement();

private:
	float BoundaryLineColor = 0.7f;
	DrawListElement DebugDrawElement;
	Shader* debugShader;
	Light* m_light;
};

