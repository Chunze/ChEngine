#include <memory>

#include "SimpleWorld.h"
#include "Light.h"
#include "Model.h"
#include "SimpleInputHandler.h"
#include "GameObject.h"
#include "RigidBox.h"
#include "RigidSphere.h"
#include "StaticMeshComponent.h"
#include "ParticleLink.h"

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
	m_Camera->SetupCamera(glm::vec3(5.0f, 5.0f, 20.0f), glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

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
	GameObject* gameObject = new GameObject(m_gameContext, this, glm::vec3(1.5f, 6.0f, 1.0f), quat(0.0f, 1.5f, 2.0f, 1.0f));
	char* nanosuit_path = "SimpleWorld/nanosuit/nanosuit.obj";
	char* crate_path = "SimpleWorld/crate/Crate1.obj";
	char* ball_path = "SimpleWorld/ball/Ball.obj";
	Model* newModel = new Model(m_gameContext, crate_path);
	Model* BallModel = new Model(m_gameContext, ball_path);
	auto staticMeshComp = GeneralStatics::NewComponent<StaticMeshComponent>(m_gameContext, this);

	staticMeshComp->SetRigidBody(std::make_shared<RigidBox>(vec3(1.0f)));
	// m_gameContext->GetPhysicsManager()->RegisterPhysicsBody(staticMeshComp->GetPhsicsBody());
	staticMeshComp->GetRigidBody()->AddCollisionPrimitive(std::make_shared<BoxPrimitive>(vec3(1.0f)));

	gameObject->SetRootComponent(staticMeshComp);
	staticMeshComp->SetMesh(newModel);
	
	staticMeshComp->GetRigidBody()->SetVelocity(vec3(0.0f, 0.0f, 0.0f));

	m_GameObjects.push_back(gameObject);

	GameObject* gameObject_1 = new GameObject(m_gameContext, this, glm::vec3(0.0f, 1.1f, 0.0));
	auto staticMeshComp_1 = GeneralStatics::NewComponent<StaticMeshComponent>(m_gameContext, this);

	staticMeshComp_1->SetRigidBody(std::make_shared<RigidBox>(vec3(1.0f)));
	//m_gameContext->GetPhysicsManager()->RegisterPhysicsBody(staticMeshComp_1->GetPhsicsBody());
	staticMeshComp_1->GetRigidBody()->AddCollisionPrimitive(std::make_shared<BoxPrimitive>(vec3(1.0f)));

	staticMeshComp_1->SetMesh(newModel);
	gameObject_1->SetRootComponent(staticMeshComp_1);

	

	m_GameObjects.push_back(gameObject_1);

	m_gameContext->GetPhysicsManager()->RegisterCollisionPrimitive(std::make_shared<SurfasePrimitive>(vec3(0.0f, 1.0f, 0.0f), 0.0f));
//	m_gameContext->GetPhysicsManager()->RegisterCollisionPrimitive(std::make_shared<SurfasePrimitive>(vec3(-1.0f, 1.0f, 0.0f), -3.0f));
// 	m_gameContext->GetPhysicsManager()->RegisterCollisionPrimitive(std::make_shared<SurfasePrimitive>(vec3(1.0f, 1.0f, 0.0f), -3.0f));
// 	m_gameContext->GetPhysicsManager()->RegisterCollisionPrimitive(std::make_shared<SurfasePrimitive>(vec3(0.0f, 1.0f, -1.0f), -3.0f));
// 	m_gameContext->GetPhysicsManager()->RegisterCollisionPrimitive(std::make_shared<SurfasePrimitive>(vec3(0.0f, 1.0f, 1.0f), -3.0f));


	//gameObject->GetRootComponent()->GetParticle()->SetVelocity(glm::vec3(0.0f, 0.0f, 0.0f));

// 	auto* CableLink = new ParticleRodLink(gameObject->GetRootComponent()->GetParticle(), 
// 										  gameObject_1->GetRootComponent()->GetParticle(),
// 										  glm::length(gameObject->GetRootComponent()->GetParticle()->GetPosition()
// 											  - gameObject_1->GetRootComponent()->GetParticle()->GetPosition()));

	//m_gameContext->GetPhysicsManager()->AddParticleContactGenerator(CableLink);

	//AnchoredSpringFG* FG = new AnchoredSpringFG(gameObject->GetWorldLocation() + glm::vec3(3.0, 5.0f, 0.0f), 10.f, 5.f);
	//FakeSprintFG* FG = new FakeSprintFG(gameObject->GetWorldLocation() + glm::vec3(3.0f, 3.0f, 0.0f), 15.0f, 0.95f);
	//m_gameContext->GetPhysicsManager()->registerForce(staticMeshComp->GetParticle(), FG);

}

void SimpleWorld::InitInputHandler()
{
	m_gameContext->m_InputHandler = new SimpleInputHandler(m_gameContext);
}
