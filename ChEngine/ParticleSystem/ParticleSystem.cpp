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
	glm::vec2 acc = glm::vec2(0.0f);
	glm::vec2 diff = glm::vec2(0.0f);
	float diffSqNorm, theta;

	float attractionCoef = 28000.0f;
	float DragCoef = 0.96f;

	for (int i = 0; i < ParticleTotalNumber; i++)
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
