#pragma once
#include "World.h"
#include <vector>

class ParticleAttractor;
class ParticleSystem;

class ParticleWorld : public World
{
public:
	ParticleWorld(GameContext* gameContext, int _particleNum);

	virtual void Update(float Delta) override;

	std::vector<ParticleAttractor*> Attractors;

	ParticleAttractor* m_DynamicAttractor;

	void CreateAttractor(int x, int y);

	void UpdateDynamicAttractor(int x, int y);

	int ParticleNum = 1;

protected:
	void InitCamera() override;
	void InitInputHandler();
	void InitParticleSystem();


	ParticleSystem* m_particleSystem;
};