#pragma once
#include "World.h"

class Model;

class SimpleWorld : public World
{
	typedef World Super;

public:
	SimpleWorld(GameContext* gameContext);
	~SimpleWorld();

	void Update(float Delta) override;
	virtual void Init() override;
	virtual void InitCamera() override;

	void InitLight();
	void InitDebugElement();


	void CreateObject();

private:
	void SetupWorld();
	void InitInputHandler();
	float BoundaryLineColor = 0.7f;
	DrawListElement DebugDrawElement;
	Shader* debugShader;
	Light* m_light;

	Model* BallModel;
};

