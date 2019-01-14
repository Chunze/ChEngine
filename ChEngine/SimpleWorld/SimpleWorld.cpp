#include "SimpleWorld.h"
#include "Light.h"
#include "Model.h"


SimpleWorld::SimpleWorld(GameContext* gameContext)
	: World(gameContext)
{
	InitCamera();
	InitWorld();
	InitDebugElement();
	InitLight();
}


SimpleWorld::~SimpleWorld()
{
}

void SimpleWorld::Update(float Delta)
{
	m_gameContext->GetDrawList()->AddToDrawQ(DebugDrawElement, false);
}

void SimpleWorld::InitWorld()
{
	GameObject* gameObject = new GameObject(m_gameContext, this, glm::vec3(1.0f, 0.0f, 0.0f));
	Model* newModel = new Model(m_gameContext, "SimpleWorld/model_1/scene.fbx");

	gameObject->AddComponent(newModel);
	m_gameObjects.push_back(gameObject);
}

void SimpleWorld::InitCamera()
{
	m_Camera = new Camera(CameraType::Camera_3D, glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	m_Camera->SetupCamera(glm::vec3(3.0f, 4.0f, 4.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	m_gameContext->GetRenderer()->SetActiveCamera(m_Camera);
}

void SimpleWorld::InitLight()
{
	m_light = new Light();
	m_light->m_position = glm::vec3(0.0f, 6.0f, 0.0f);
	m_light->m_ambient = glm::vec3(0.2f, 0.2f, 0.2f);
	m_light->m_diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	m_light->m_specular = glm::vec3(1.0f, 1.0f, 1.0f);
	m_gameContext->GetRenderer()->SetLight(m_light);
}

void SimpleWorld::InitDebugElement()
{
	static float axisLineVertices[] = {
		0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		5.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 5.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 5.0f, 0.0f, 0.0f, 1.0f,
	};

	debugShader = new Shader("DebugDrawShader.vert", "DebugDrawShader.frag");
	DebugDrawElement.vertexBuffer = axisLineVertices;
	DebugDrawElement.drawingPrimitive = DrawingPrimitives::LINES;
	DebugDrawElement.shader = *debugShader;
	DebugDrawElement.VBsize_inByte = sizeof(axisLineVertices);
	DebugDrawElement.attributeSizes.push_back(3);
	DebugDrawElement.attributeSizes.push_back(3);
	DebugDrawElement.vertextInfoSize = 6;
	DebugDrawElement.bIsDebug = true;
	DebugDrawElement.LineWidth = 3;
}
