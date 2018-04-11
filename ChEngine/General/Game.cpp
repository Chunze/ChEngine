#include <iostream>
#include <string>
#include "Game.h"
#include "Jello.h"
#include "pic.h"

// components and managers for the game
#include "Renderer.h"
#include "PhysicsManager.h"
#include "JelloWorld.h"
#include "ParticleWorld.h"
#include "InputHandler.h"
#include "HW3/IKWorld.h"

#define SCREEN_SHOTS 0
GameContext* gameContext;
double mouselastX = 400, mouselastY = 300;
bool firstMouse = true;
int* WindowWidth_callback;
int* WindowHeight_callback;

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse) // this bool variable is initially set to true
	{
		mouselastX = xpos;
		mouselastY = ypos;
		firstMouse = false;
	}

	float offsetX = (float)(xpos - mouselastX);
	float offsetY = (float)(mouselastY - ypos);
	mouselastX = xpos;
	mouselastY = ypos;

	float sensitivity = 0.05f;
	offsetX *= sensitivity;
	offsetY *= sensitivity;

	gameContext->m_renderer->mainCamera->Rotate(offsetY, offsetX);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	glfwGetWindowSize(gameContext->GetWindow(), WindowWidth_callback, WindowHeight_callback);
}

Game::Game(GameContext* _gameContext)
	: BaseClass(_gameContext)
{
	
}

void Game::InitGame()
{
	std::cout << "Initializing game...\n";

	// add callback function for resizing the window
	WindowWidth_callback = &WindowWidth;
	WindowHeight_callback = &WindowHeight;
	glfwSetFramebufferSizeCallback(m_gameContext->GetWindow(), framebuffer_size_callback);
	gameContext = m_gameContext;

	{
		m_gameContext->m_Game = this;
		glfwGetWindowSize(m_gameContext->GetWindow(), &WindowWidth, &WindowHeight);
	}

	// creating components and managers for the game
	{
		DrawList* drawList = new DrawList(m_gameContext);
		m_gameContext->m_drawList = drawList;
	}
	{
		std::cout << "Initializing renderer...\n";
		Renderer* renderer = new Renderer(m_gameContext);
		m_gameContext->m_renderer = renderer;
	}
	{
		std::cout << "Initializing physics manager...\n";
		PhysicsManager* physicsManager = new PhysicsManager(m_gameContext);
		m_gameContext->m_physicsManager = physicsManager;
	}
	{
		std::cout << "Enter homework number (1, 2)...\n";
		std::string Homework = "3";
		//std::cin >> Homework;



		if (Homework == "1")
		{
			m_gameContext->m_renderer->SetBackgroundColor(0.2f, 0.3f, 0.3f);

			World* world = new JelloWorld(m_gameContext);
			std::cout << "Enter Jello World file path (cs520/...)...\n";
			std::string WorldFile;
			std::cin >> WorldFile;
			
			while (!world->LoadWorld(WorldFile.c_str()))
			{
				std::cout << "Invalid input... Enter Jello World file path (cs520/...)...\n";
				std::cin >> WorldFile;
			}

			std::cout << "Loading world...\n";
			m_gameContext->m_world = world;
		}
		else if (Homework == "2")
		{
			m_gameContext->m_renderer->SetBackgroundColor(0.0f, 0.0f, 0.0f);
			std::cout << "Enter number of particles...\n";
			int ParticleNum;
			std::cin >> ParticleNum;

			World* world = new ParticleWorld(m_gameContext, ParticleNum);
			m_gameContext->m_world = world;

			bPaused = false;

			static_cast<ParticleWorld*>(world)->UpdateDynamicAttractor(WindowWidth, WindowHeight);
		}
		else if (Homework == "3")
		{
			m_gameContext->m_renderer->SetBackgroundColor(0.0f, 0.0f, 0.0f);

			World* world = new IKWorld(m_gameContext);
			m_gameContext->m_world = world;
			bPaused = false;
		}
		else if (Homework == "0")
		{
// 			m_gameContext->m_renderer->SetBackgroundColor(0.2f, 0.3f, 0.3f);
// 			World* world = new JelloWorld(m_gameContext);
// 			world->LoadWorld("cs520/moveLeft.w");

			m_gameContext->m_renderer->SetBackgroundColor(0.0f, 0.0f, 0.0f);
			World* world = new ParticleWorld(m_gameContext, 10000);

			bPaused = false;

			m_gameContext->m_world = world;
		}
	}
}

void Game::GameLoop()
{
	while (!glfwWindowShouldClose(m_gameContext->GetWindow()))
	{
		bWasPausedLastFrame = bPaused;
		float currentTime = (float)glfwGetTime();
		deltaTime = currentTime - lastFrameTime;

		if (ShowFPS)
		{
			FPS = 1 / deltaTime;
			char buf[10];
			sprintf(buf, "%.2f", FPS);
			std::string fps = std::string(buf);

			m_gameContext->GetDrawList()->AddOnScreenText("FPS: " + fps, 25.0f, 25.0f, 1.0f, glm::vec3(0.9, 0.9, 0.9));
		}

		// First frame
		if (lastFrameTime == 0.0f)
		{
			deltaTime = 0.0005f;
		}

		lastFrameTime = currentTime;

		// input
		processInput(m_gameContext->GetWindow());

		float TimeSinceLastScreenSave = currentTime - lastScreenCapture;

		float ScreenCaptureRate = 1.0f / 15.0f;

		if (SCREEN_SHOTS && deltaTime != 0.0f && ScreenCaptureIndex < 250 && TimeSinceLastScreenSave > ScreenCaptureRate)
		{
			TimeSinceLastScreenSave = currentTime;
			char fileName[] = "../../cs520_ScreenShots/xxx.ppm";
			
			fileName[24] = '0' + (ScreenCaptureIndex / 100);
			fileName[25] = '0' + ((ScreenCaptureIndex % 100) / 10);
			fileName[26] = '0' + (ScreenCaptureIndex % 10);
			
			saveScreenshot(WindowWidth, WindowHeight, fileName);

			ScreenCaptureIndex++;
		}

		if (m_gameContext->GetWorld() && m_gameContext->GetWorld()->m_customDelta > 0.0f)
		{
			deltaTime = m_gameContext->GetWorld()->m_customDelta;
		}

		if (bPaused)
		{
			deltaTime = 0.0f;
			bWasPausedLastFrame = false;
		}
		Update(deltaTime);
		
		glfwSwapBuffers(m_gameContext->GetWindow());
		glfwPollEvents();
	}
}

void Game::processInput(GLFWwindow* contextWindow)
{
	if (m_gameContext->GetInputHandler() == nullptr)
	{
		return;
	}

	m_gameContext->GetInputHandler()->ProcessInput(contextWindow);
}

void Game::saveScreenshot(int windowWidth, int windowHeight, char *filename)
{
	if (filename == NULL)
		return;

	// Allocate a picture buffer 
	Pic * in = pic_alloc(windowWidth, windowHeight, 3, NULL);

	//printf("File to save to: %s\n", filename);

	for (int i = windowHeight - 1; i >= 0; i--)
	{
		glReadPixels(0, windowHeight - i - 1, windowWidth, 1, GL_RGB, GL_UNSIGNED_BYTE,
			&in->pix[i*in->nx*in->bpp]);
	}

	if (ppm_write(filename, in))
		printf("File saved Successfully\n");
	else
		printf("Error in Saving\n");

	pic_free(in);
}

void Game::Update(float Delta)
{

	int IntegrationMode = m_gameContext->GetPhysicsManager()->GetIntegrator();
	if (IntegrationMode == 0)
	{
		// force update
		m_gameContext->GetPhysicsManager()->UpdateForces(Delta);

		// world update
		m_gameContext->GetWorld()->Update(Delta);

		// physics update
		m_gameContext->GetPhysicsManager()->UpdateContactForces(Delta);
	}
	else if (IntegrationMode == 1)
	{

		while (m_gameContext->GetPhysicsManager()->GetRK4StepCount() <= 4)
		{
			// force update
			m_gameContext->GetPhysicsManager()->UpdateForces(Delta);
			// world update (Particles will check current integrate method, and act accordingly)
			m_gameContext->GetWorld()->Update(Delta);
			// physics update
			m_gameContext->GetPhysicsManager()->UpdateContactForces(Delta);
			// Update RK4 step, very important for the particle to know what stage the update is in
			m_gameContext->GetPhysicsManager()->Increment_RK4_step();
		}

		m_gameContext->GetPhysicsManager()->Reset_RK4_Step();
	}


	// render update
	m_gameContext->m_renderer->Update(deltaTime);
}

