#pragma once
#include <vector>
#include "glm.h"
#include "GameObject.h"
#include "Particle.h"


class Jello : public GameObject
{
public:

	Jello(GameContext* gameContext, World* world, glm::vec3 position = glm::vec3(0.0f), float size = 1.0f);
	~Jello();

	glm::vec3 m_position;
	float m_size;

	// mass for each particle
	float m_mass;
	float m_step;

	Particle m_particles[8][8][8];

	float m_kElastic; // Hook's elasticity coefficient for all springs except collision springs
	float m_dElastic; // Damping coefficient for all springs except collision springs
	

	void InitJello();
	void CreateSprings();

	virtual void CreateAndAddDrawListElement(int Mode);
	virtual void Update(float Delta);

	bool drawListElementValid = false;
protected:
	DrawListElement e;
	DrawListElement ParticleRender;
	DrawListElement SpringRender;

	int m_drawListIndex = -1;

	// For triangle mode
	void UpdateVertex();
	float Vertices[4032];

	// For wire frame mode
	void UpdateParticleRenderInfo();
	float ParticleRenderInfo[1776];

	void UpdateSpringRenderInfo();
	std::vector<float> SpringRenderInfo;

private:
	int debug;
};