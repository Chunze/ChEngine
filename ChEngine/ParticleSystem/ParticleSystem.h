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

	void Initialize(int Num_Particles);

	void UpdateParticles(float Delta);
	void UpdateVertexBuffer();

protected:

	float GetSpeedCoef(glm::vec2 speed);
	float getValue(float coef);
	float getSaturation(float coef);
	float getHue(float coef);
	glm::vec3 hsv2rgb(float h, float s, float v);
private:
	float attractionCoef = 28000.0f;
	float DragCoef = 0.96f;

	float slowValue = 0.0f;
	float fastValue = 1.0f;
	float slowSaturation = 0.0f;
	float fastSaturation = 1.0f;
	float slowHue = 1.0f;
	float fastHue = 0.0f;
};