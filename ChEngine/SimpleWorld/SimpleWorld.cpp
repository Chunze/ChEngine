#include "SimpleWorld.h"
#include "Light.h"
#include "Model.h"
#include "JelloWorldInputHandler.h"
#include "GameObject.h"
#include "StaticMeshComponent.h"

#include "AnchoredSpringFG.h"
#include "FakeSprintFG.h"
#include "PhysicsManager.h"


SimpleWorld::SimpleWorld(GameContext* gameContext)
	: Super(gameContext)
{
	Init();
}


SimpleWorld::~SimpleWorld()
{
}

void SimpleWorld::Update(float Delta)
{
	Super::Update(Delta);
	m_gameContext->GetDrawList()->AddToDrawQ(DebugDrawElement, false);
}

void SimpleWorld::Init()
{
	Super::Init();

	InitCamera();
	InitDebugElement();
	InitLight();
	InitInputHandler();

	SetupWorld();
}

void SimpleWorld::InitCamera()
{
	m_Camera = new Camera(CameraType::Camera_3D, glm::radians(45.0f), 16.0f / 9.0f, 0.1f, 100.0f);
	m_Camera->SetupCamera(glm::vec3(30.0f, 15.0f, 30.0f), glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	m_gameContext->GetRenderer()->SetActiveCamera(m_Camera);
}

void SimpleWorld::InitLight()
{
	m_light = new Light();
	m_light->m_position = glm::vec3(0.0f, 40.0f, 0.0f);
	m_light->m_ambient = glm::vec3(0.2f, 0.2f, 0.2f);
	m_light->m_diffuse = glm::vec3(0.6f, 0.6f, 0.6f);
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

void SimpleWorld::SetupWorld()
{
	GameObject* gameObject = new GameObject(m_gameContext, this, glm::vec3(0.0f, 6.0f, 0.0f));
	char* nanosuit_path = "SimpleWorld/nanosuit/nanosuit.obj";
	char* crate_path = "SimpleWorld/crate/Crate1.obj";
	char* ball_path = "SimpleWorld/ball/Ball.obj";
	Model* newModel = new Model(m_gameContext, crate_path);
	StaticMeshComponent* staticMeshComp = new StaticMeshComponent(m_gameContext, this);
	gameObject->SetRootComponent(staticMeshComp);
	staticMeshComp->SetMesh(newModel);

	m_GameObjects.push_back(gameObject);
	m_SceneObjects.push_back(staticMeshComp);

	//AnchoredSpringFG* FG = new AnchoredSpringFG(gameObject->GetWorldLocation() + glm::vec3(3.0, 5.0f, 0.0f), 10.f, 5.f);
	//FakeSprintFG* FG = new FakeSprintFG(gameObject->GetWorldLocation() + glm::vec3(3.0f, 3.0f, 0.0f), 15.0f, 0.95f);
	//m_gameContext->GetPhysicsManager()->registerForce(staticMeshComp->GetParticle(), FG);

}

void SimpleWorld::InitInputHandler()
{
	m_gameContext->m_InputHandler = new JelloWorldInputHandler(m_gameContext);
}
