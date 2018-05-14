#include "IKWorld.h"
#include "TailSkel.h"
#include "HW3InputHandler.h"

#include "MathStatics.h"

#define FAST_IK_ITERATION 20
#define SLOW_IK_ITERATION 400
#define IK_CCD_ROTATION_FRACTION 1 / 10

IKWorld::IKWorld(GameContext* gameContext)
	: World(gameContext)
{
	
	InitCamera();
	InputHandler();
}

void IKWorld::Update(float Delta)
{
	if (CCD_running)
	{
		IKSolver_CCD_Slow(target_x, target_y, CCD_accept_distance);
		CCD_Iteration++;
		if (CCD_Iteration > SLOW_IK_ITERATION)
		{
			CCD_running = false;
		}
	}
	Tail->Update(Delta);
}

void IKWorld::InitCamera()
{
	m_Camera = new Camera(CameraType::Camera_2D, 0.0f, static_cast<float>(m_gameContext->GetGame()->WindowWidth), 0.0f, static_cast<float>(m_gameContext->GetGame()->WindowHeight));
	m_Camera->SetupCamera(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	m_gameContext->GetRenderer()->SetActiveCamera(m_Camera);
}

void IKWorld::InitBones(int NumOfBones)
{
	Tail = new TailSkel(m_gameContext, this, NumOfBones);
}

void IKWorld::InputHandler()
{
	m_gameContext->m_InputHandler = new HW3InputHandler(m_gameContext);
}

void IKWorld::IKSolver_CCD_Fast(float x, float y, float accept_distance)
{
	size_t num_bones = Tail->Structure.size();
	glm::vec2 last_ef_position(10000.0f);

	glm::vec2 target = glm::vec2(x, y);

	int iteration = 0;
	// doing CCD as fast as possible (make biggest rotation each time)
		
	while(iteration < FAST_IK_ITERATION)
	{
		iteration++;
		for (int index = num_bones - 1; index >= 0; index--)
		{
			float turning_angle = Cal_RotateAngle_CCD(index, target);

			Tail->RotateBone(index, turning_angle);

			// checking if the end effector has moved after this iteration
			float diff = glm::length(last_ef_position - Tail->GetEndEffectorPos());
			if (diff <= SMALL_NUMBER)
			{
				break;
			}
			else
			{
				last_ef_position = Tail->GetEndEffectorPos();
			}

			// checking if end effector reaches the target
			glm::vec2 ef_target = target - Tail->GetEndEffectorPos();
			float distSq = ef_target.x * ef_target.x + ef_target.y * ef_target.y;
			if (distSq <= accept_distance * accept_distance)
			{
				break;
			}
		}
	}
}

void IKWorld::IKSolver_CCD_Slow(float x, float y, float accept_distance)
{
	// doing CCD with the animation (rotate a little each frame)
	size_t num_bones = Tail->Structure.size();
	glm::vec2 last_ef_position(10000.0f);
	glm::vec2 target = glm::vec2(x, y);

	for (int index = num_bones - 1; index >= 0; index--)
	{
		float turning_angle = Cal_RotateAngle_CCD(index, target)* IK_CCD_ROTATION_FRACTION;

		if (turning_angle > 0)
		{
			Math::Clamp(turning_angle, 0.2f, 20.0f);
		}
		else
		{
			Math::Clamp(turning_angle, -20.0f, -0.2f);
		}
		

		Tail->RotateBone(index, turning_angle);

		// checking if the end effector has moved after this iteration
		float diff = glm::length(last_ef_position - Tail->GetEndEffectorPos());
		if (diff <= SMALL_NUMBER)
		{
			CCD_running = false;
			break;
		}
		else
		{
			last_ef_position = Tail->GetEndEffectorPos();
		}

		// checking if end effector reaches the target
		glm::vec2 ef_target = target - Tail->GetEndEffectorPos();
		float distSq = ef_target.x * ef_target.x + ef_target.y * ef_target.y;
		if (distSq <= accept_distance * accept_distance)
		{
			CCD_running = false;
			break;
		}
	}
}

float IKWorld::Cal_RotateAngle_CCD(int index, glm::vec2 target)
{
	Bone current_bone = Tail->Structure[index];
	glm::vec2 joint_target = target - current_bone.Position_World;
	joint_target = glm::normalize(joint_target);

	// vector from joint position to end effector
	glm::vec2 joint_ef = Tail->GetEndEffectorPos() - current_bone.Position_World;
	joint_ef = glm::normalize(joint_ef);

	float cosine = glm::dot(joint_ef, joint_target);
	float sine = Math::Cross(joint_ef, joint_target);

	float angle = Math::GetAngleInDegree(sine, cosine);

	return angle;
}

void IKWorld::Start_IK_Solver_CCD(float x, float y, float accept_distance, int mode /*= 0*/)
{
	if (mode == 0)
	{
		IKSolver_CCD_Fast(x, y, accept_distance);
	}
	else if (mode == 1)
	{
		CCD_running = true;
		CCD_Iteration = 0;
		CCD_accept_distance = accept_distance;
		target_x = x;
		target_y = y;
	}
}
