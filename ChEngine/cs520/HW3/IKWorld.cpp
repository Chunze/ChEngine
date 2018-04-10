#include "IKWorld.h"
#include "TailSkel.h"

IKWorld::IKWorld(GameContext* gameContext)
	: World(gameContext)
{
	Tail = new TailSkel(gameContext, this);
	InitCamera();
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
