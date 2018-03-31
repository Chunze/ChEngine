#pragma once
#include "GameObject.h"


class ParticleSystem : public GameObject
{
public:

	ParticleSystem(GameContext* gameContext, World* world);

	~ParticleSystem();

	void Update(float Delta) override;

	int ParticleTotalNumber;

	glm::vec2* particlePositions;
	glm::vec2* particleVelocities;
	glm::vec3* particleColors;

	float* VertexBuffer;

	DrawListElement e;

	void Initialize(int Num_Particles);

	void UpdateParticles(float Delta);
	void UpdateVertexBuffer();

private:
	float a;
};