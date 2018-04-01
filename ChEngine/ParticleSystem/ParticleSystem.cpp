#include <vector>
#include "ParticleSystem.h"
#include "ParticleAttractor.h"
#include "ParticleWorld.h"
#include "glm.h"
#include "Game.h"

ParticleSystem::ParticleSystem(GameContext* gameContext, World* world)
	: GameObject(gameContext, world)
{

}

ParticleSystem::~ParticleSystem()
{
	delete particlePositions;
	delete particleVelocities;
	delete particleColors;
	delete VertexBuffer;
}

void ParticleSystem::Update(float Delta)
{
	UpdateParticles(Delta);
	UpdateVertexBuffer();
	m_gameContext->GetDrawList()->AddToDrawQ(e, false);
}

void ParticleSystem::Initialize(int Num_Particles)
{
	ParticleTotalNumber = Num_Particles;
	particlePositions = new glm::vec2[ParticleTotalNumber];
	particleVelocities = new glm::vec2[ParticleTotalNumber];
	particleColors = new glm::vec3[ParticleTotalNumber];

	VertexBuffer = new float[ParticleTotalNumber * 5];

	int window_width = m_gameContext->GetGame()->WindowWidth;
	int window_height = m_gameContext->GetGame()->WindowHeight;

	// For testing
	for (int i = 0; i < ParticleTotalNumber; i++)
	{
		float rand_width = static_cast<float>(rand()) / (static_cast <float> (RAND_MAX / window_width));
		float rand_height = static_cast<float>(rand()) / (static_cast <float> (RAND_MAX / window_height));
		particlePositions[i] = glm::vec2(rand_width, rand_height);
		particleVelocities[i] = glm::vec3(0.0f);
		particleColors[i] = glm::vec3(1.0f, 1.0f, 1.0f);
	}

	e.attributeSizes.push_back(2);
	e.attributeSizes.push_back(3);
	e.drawingPrimitive = DrawingPrimitives::POINTS;
	e.PointSize = 1;
	e.shader = *(new Shader("ScreenSpaceParticle.vert", "ScreenSpaceParticle.frag"));
	e.vertextInfoSize = 5;
	e.bIsDebug = true;

	UpdateVertexBuffer();

	e.vertexBuffer = VertexBuffer;
	e.VBsize_inByte = ParticleTotalNumber * e.vertextInfoSize * sizeof(float);
}

void ParticleSystem::UpdateParticles(float Delta)
{
	std::vector<ParticleAttractor*> Attractors = static_cast<ParticleWorld*>(GetWorld())->Attractors;
	glm::vec3 color = glm::vec3(0.0f);
	glm::vec2 acc = glm::vec2(0.0f);
	glm::vec2 diff = glm::vec2(0.0f);
	float diffSqNorm, theta;
	
	float SpeedCoef = 0.0f;

	for (int i = ParticleTotalNumber - 1; i >= 0; i--)
	{
		for (auto attractor : Attractors)
		{
			if (!attractor->bIsActive) { continue; }

			diff = attractor->Position - particlePositions[i];
			diffSqNorm = diff.x * diff.x + diff.y * diff.y;

			if (diffSqNorm < 0.1f)
			{
				theta = static_cast<float>(rand()) / (static_cast <float> (RAND_MAX / 6.28318530718f));
				theta *= 180.f;
				diff.x = cos(theta);
				diff.y = sin(theta);
				diffSqNorm = 1;
			}

			acc += (attractionCoef / diffSqNorm) * diff;
		}

		particleVelocities[i] += Delta * acc;
		particlePositions[i] += Delta * particleVelocities[i];
		particleVelocities[i] *= DragCoef;


		SpeedCoef = GetSpeedCoef(particleVelocities[i]);
		color = hsv2rgb(getHue(SpeedCoef), getSaturation(SpeedCoef), getValue(SpeedCoef));

		particleColors[i] = color;

		acc.x = 0.0f;
		acc.y = 0.0f;
	}
	
}

void ParticleSystem::UpdateVertexBuffer()
{
	int index = 0;
	for (int i = 0; i < ParticleTotalNumber; i++)
	{
		VertexBuffer[index++] = particlePositions[i].x;
		VertexBuffer[index++] = particlePositions[i].y;
		VertexBuffer[index++] = particleColors[i].x;
		VertexBuffer[index++] = particleColors[i].y;
		VertexBuffer[index++] = particleColors[i].z;
	}
}

float ParticleSystem::GetSpeedCoef(glm::vec2 speed)
{
	float coef;
	coef = log(speed.x * speed.x + speed.y * speed.y + 1) / 9.0f;  // Use + 1 to have positive log values.
	if (coef > 1.0f) {
		coef = 1.0f;
	}
	return coef;
}

float ParticleSystem::getValue(float coef)
{
	return (1 - coef) * slowValue + coef * fastValue;
}

float ParticleSystem::getSaturation(float coef)
{
	return (1 - coef) * slowSaturation + coef * fastSaturation;
}

float ParticleSystem::getHue(float coef)
{
	float hue;
// 	float sh = slowHue;
// 	float fh = fastHue;
// 	if (sh < fh && hueDirection == 0) {
// 		sh += 1;
// 	}
// 	else if (sh > fh && hueDirection == 1) {
// 		fh += 1;
// 	}
	hue = (1 - coef) * slowHue + coef * fastHue;
	if (hue >= 1) {
		hue -= 1;
	}
	return hue;
}

glm::vec3 ParticleSystem::hsv2rgb(float h, float s, float v) {
	glm::vec3 rgb;
	float h6 = 6 * h;
	float r, g, b;  // NOT the actual rgb values.
	float coef;

	if (h6 < 1) {
		r = 0;
		g = 1 - h6;
		b = 1;
	}
	else if (h6 < 2) {
		r = h6 - 1;
		g = 0;
		b = 1;
	}
	else if (h6 < 3) {
		r = 1;
		g = 0;
		b = 3 - h6;
	}
	else if (h6 < 4) {
		r = 1;
		g = h6 - 3;
		b = 0;
	}
	else if (h6 < 5) {
		r = 5 - h6;
		g = 1;
		b = 0;
	}
	else {
		r = 0;
		g = 1;
		b = h6 - 5;
	}

	coef = v * s;
	rgb.r = v - coef * r;
	rgb.g = v - coef * g;
	rgb.b = v - coef * b;
	return rgb;
}