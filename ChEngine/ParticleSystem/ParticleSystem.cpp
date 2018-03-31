#include "ParticleSystem.h"
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
	a += Delta;
	float s = sin(10*a);

	int index = 0;
	for (int i = 0; i < ParticleTotalNumber; i++)
	{
		VertexBuffer[index++] = particlePositions[i].x + s;
		VertexBuffer[index++] = particlePositions[i].y;
		VertexBuffer[index++] = particleColors[i].x;
		VertexBuffer[index++] = particleColors[i].y;
		VertexBuffer[index++] = particleColors[i].z;
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
