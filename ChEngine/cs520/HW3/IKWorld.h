#ifndef IK_WORLD_H
#define IK_WORLD_H

#include "World.h"

class IKWorld : public World
{
public:
	IKWorld(GameContext* gameContext);

	virtual void Update(float Delta) override;

	virtual void InitCamera() override;

	void InitBones(int NumOfBones);

	void InputHandler();

	void IKSolver_CCD_Fast(float x, float y, float accept_distance);

	void IKSolver_CCD_Slow(float x, float y, float accept_distance);

	float Cal_RotateAngle_CCD(int index, glm::vec2 target);

	void Start_IK_Solver_CCD(float x, float y, float accept_distance, int mode = 0);
	int CCD_Iteration = 0;

	bool CCD_running;

	float target_x, target_y, CCD_accept_distance;

	class TailSkel* Tail;
};

#endif
