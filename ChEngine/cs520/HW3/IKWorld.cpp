#include "IKWorld.h"
#include "TailSkel.h"
#include "HW3InputHandler.h"

#include "MathStatics.h"

#define IK_ITERATION 20

IKWorld::IKWorld(GameContext* gameContext)
	: World(gameContext)
{
	Tail = new TailSkel(gameContext, this, 5);
	InitCamera();
	InputHandler();
}

void IKWorld::Update(float Delta)
{
	Tail->Update(Delta);
}

void IKWorld::InitCamera()
{
	m_Camera = new Camera(CameraType::Camera_2D, 0.0f, static_cast<float>(m_gameContext->GetGame()->WindowWidth), 0.0f, static_cast<float>(m_gameContext->GetGame()->WindowHeight));
	m_Camera->SetupCamera(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	m_gameContext->GetRenderer()->SetActiveCamera(m_Camera);
}

void IKWorld::InputHandler()
{
	m_gameContext->m_InputHandler = new HW3InputHandler(m_gameContext);
}

void IKWorld::IKSolver_CCD(float x, float y, float accept_distance, int mode)
{
	if (mode == 0)
	{
		// doing CCD as fast as possible (make biggest rotation each time)
		size_t num_bones = Tail->Structure.size();
		glm::vec2 last_ef_position(10000.0f);

		int iteration = 0;
		while(iteration < IK_ITERATION)
		{
			iteration++;
			for (int index = num_bones - 1; index >= 0; index--)
			{
				Bone current_bone = Tail->Structure[index];
				glm::vec2 target = glm::vec2(x, y);
				glm::vec2 joint_target = target - current_bone.Position_World;
				joint_target = glm::normalize(joint_target);

				// vector from joint position to end effector
				glm::vec2 joint_ef = Tail->GetEndEffectorPos() - current_bone.Position_World;
				joint_ef = glm::normalize(joint_ef);

				float cosine = glm::dot(joint_ef, joint_target);
				float sine = Math::Cross(joint_ef, joint_target);

				float angle = Math::GetAngleInDegree(sine, cosine);

				Tail->RotateBone(index, angle);

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
}
