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

	void LeftClicked();

	int ParticleNum = 1;

	
protected:
	void InitCamera() override;
	void InitInputHandler();
	void InitParticleSystem();

	void ClearAttractors();

	int NumLeftClicks = 0;

	int MaxAttractorNum = 4;
	int AttractorToBeModified = 0;
	ParticleSystem* m_particleSystem;
};