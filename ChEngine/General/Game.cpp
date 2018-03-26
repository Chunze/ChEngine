#include <iostream>
#include <string>
#include "Game.h"
#include "Jello.h"
#include "pic.h"

// components and managers for the game
#include "Renderer.h"
#include "PhysicsManager.h"
#include "JelloWorld.h"

#define SCREEN_SHOTS 0
GameContext* gameContext;
double mouselastX = 400, mouselastY = 300;
bool firstMouse = true;

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

Game::Game(GameContext* gameContext)
	: BaseClass(*gameContext)
{
	
}

void Game::InitGame()
{
	cout << "Initializing game...\n";
	// creating components and managers for the game
	{
		DrawList* drawList = new DrawList(m_gameContext);
		m_gameContext.m_drawList = drawList;
	}
	{
		cout << "Initializing renderer...\n";
		Renderer* renderer = new Renderer(m_gameContext);
		m_gameContext.m_renderer = renderer;
	}
	{
		cout << "Initializing physics manager...\n";
		PhysicsManager* physicsManager = new PhysicsManager(m_gameContext);
		m_gameContext.m_physicsManager = physicsManager;
	}
	{
		cout << "Enter homework number (HW1, HW2)...\n";
		std::string Homework;
		cin >> Homework;
		if (Homework == "HW1")
		{
			World* world = new JelloWorld(m_gameContext);
			cout << "Enter Jello World file path (cs520/...)...\n";
			std::string WorldFile;
			cin >> WorldFile;

			
			while (!world->LoadWorld(WorldFile.c_str()))
			{
				cout << "Invalid input... Enter Jello World file path (cs520/...)...\n";
				cin >> WorldFile;
			}

			cout << "Loading world...\n";
			m_gameContext.m_world = world;
		}
		else if (Homework == "HW2")
		{

		}
		else if (Homework == "0")
		{
			World* world = new JelloWorld(m_gameContext);
			world->LoadWorld("cs520/moveLeft.w");
			m_gameContext.m_world = world;
		}
	}
}

void Game::GameLoop()
{
	GLFWwindow* contextWindow = m_gameContext.m_contextWindow;
	gameContext = &m_gameContext;
	

	while (!glfwWindowShouldClose(contextWindow))
	{
		bWasPausedLastFrame = bPaused;
		float currentTime = (float)glfwGetTime();
		deltaTime = currentTime - lastFrameTime;

		FPS = 1 / deltaTime;

		// First frame
		if (lastFrameTime == 0.0f)
		{
			deltaTime = 0.0005f;
		}

		lastFrameTime = currentTime;

		// input
		processInput(contextWindow);


		if (bPaused)
		{
			deltaTime = 0.0f;
			bWasPausedLastFrame = false;
		}

		
		float TimeSinceLastScreenSave = currentTime - lastScreenCapture;

		float ScreenCaptureRate = 1.0f / 15.0f;

		if (SCREEN_SHOTS && deltaTime != 0.0f && ScreenCaptureIndex < 250 && TimeSinceLastScreenSave > ScreenCaptureRate)
		{
			TimeSinceLastScreenSave = currentTime;
			char fileName[] = "../../cs520_ScreenShots/xxx.ppm";
			
			fileName[24] = '0' + (ScreenCaptureIndex / 100);
			fileName[25] = '0' + ((ScreenCaptureIndex % 100) / 10);
			fileName[26] = '0' + (ScreenCaptureIndex % 10);

			int WindowWidth, WindowHeight;
			glfwGetWindowSize(contextWindow, &WindowWidth, &WindowHeight);
			saveScreenshot(WindowWidth, WindowHeight, fileName);

			ScreenCaptureIndex++;
		}

		deltaTime = 0.0005f;
		if (bPaused)
		{
			deltaTime = 0.0f;
			//bGamePaused = true;
		}
		Update(deltaTime);
		
		glfwSwapBuffers(contextWindow);
		glfwPollEvents();
	}
}

void Game::processInput(GLFWwindow* contextWindow)
{
	if (glfwGetKey(contextWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(contextWindow, true);
	}

	if (glfwGetKey(contextWindow, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		KEY_SPACE_WasPressed = true;
	}

	if (glfwGetKey(contextWindow, GLFW_KEY_SPACE) == GLFW_RELEASE)
	{
		if (KEY_SPACE_WasPressed)
		{
			KEY_SPACE_WasPressed = false;
			bPaused = !bPaused;
		}
	}

	if (glfwGetMouseButton(contextWindow, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		glfwSetInputMode(contextWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetCursorPosCallback(contextWindow, mouse_callback);
		
		if (glfwGetKey(contextWindow, GLFW_KEY_W) == GLFW_PRESS)
		{
			m_gameContext.m_renderer->FlyCameraForward(1.0f);
		}
		if (glfwGetKey(contextWindow, GLFW_KEY_A) == GLFW_PRESS)
		{
			m_gameContext.m_renderer->FlyCameraRight(-1.0f);
		}
		if (glfwGetKey(contextWindow, GLFW_KEY_S) == GLFW_PRESS)
		{
			m_gameContext.m_renderer->FlyCameraForward(-1.0f);
		}
		if (glfwGetKey(contextWindow, GLFW_KEY_D) == GLFW_PRESS)
		{
			m_gameContext.m_renderer->FlyCameraRight(1.0f);
		}
		if (glfwGetKey(contextWindow, GLFW_KEY_Q) == GLFW_PRESS)
		{
			m_gameContext.m_renderer->FlyCameraUp(-1.0f);
		}
		if (glfwGetKey(contextWindow, GLFW_KEY_E) == GLFW_PRESS)
		{
			m_gameContext.m_renderer->FlyCameraUp(1.0f);
		}
	}

	if (glfwGetMouseButton(contextWindow, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
	{
		// mouse right release, show cursor, unbind callback function and reset
		glfwSetInputMode(contextWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		glfwSetCursorPosCallback(contextWindow, NULL);
		firstMouse = true;
	}
	
	if (glfwGetKey(contextWindow, GLFW_KEY_F4) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	if (glfwGetKey(contextWindow, GLFW_KEY_F5) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	if (glfwGetKey(contextWindow, GLFW_KEY_V) == GLFW_PRESS)
	{
		KEY_V_WasPressed = true;
	}
	if (glfwGetKey(contextWindow, GLFW_KEY_V) == GLFW_RELEASE)
	{
		if (KEY_V_WasPressed)
		{
			KEY_V_WasPressed = false;
			JelloWorld* _jelloWorld = static_cast<JelloWorld*>(m_gameContext.GetWorld());
			if (_jelloWorld)
			{
				_jelloWorld->ToggleDrawingMode();
			}
		}
	}
	if (glfwGetKey(contextWindow, GLFW_KEY_X) == GLFW_PRESS)
	{
		KEY_X_WasPressed = true;
	}
	if (glfwGetKey(contextWindow, GLFW_KEY_X) == GLFW_RELEASE)
	{
		if (KEY_X_WasPressed)
		{
			KEY_X_WasPressed = false;
			JelloWorld* _jelloWorld = static_cast<JelloWorld*>(m_gameContext.GetWorld());
			if (_jelloWorld)
			{
				_jelloWorld->ToggleStructuralSpring();
			}
		}
	}

	if (glfwGetKey(contextWindow, GLFW_KEY_H) == GLFW_PRESS)
	{
		KEY_H_WasPressed = true;
	}
	if (glfwGetKey(contextWindow, GLFW_KEY_H) == GLFW_RELEASE)
	{
		if (KEY_H_WasPressed)
		{
			KEY_H_WasPressed = false;
			JelloWorld* _jelloWorld = static_cast<JelloWorld*>(m_gameContext.GetWorld());
			if (_jelloWorld)
			{
				_jelloWorld->ToggleShearSpring();
			}
		}
	}

	if (glfwGetKey(contextWindow, GLFW_KEY_B) == GLFW_PRESS)
	{
		KEY_B_WasPressed = true;
	}
	if (glfwGetKey(contextWindow, GLFW_KEY_B) == GLFW_RELEASE)
	{
		if (KEY_B_WasPressed)
		{
			KEY_B_WasPressed = false;
			JelloWorld* _jelloWorld = static_cast<JelloWorld*>(m_gameContext.GetWorld());
			if (_jelloWorld)
			{
				_jelloWorld->ToggleBendSpring();
			}
		}
	}
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

	int IntegrationMode = m_gameContext.GetPhysicsManager()->GetIntegrator();
	if (IntegrationMode == 0)
	{
		// force update
		m_gameContext.GetPhysicsManager()->UpdateForces(Delta);

		// world update
		m_gameContext.GetWorld()->Update(Delta);

		// physics update
		m_gameContext.GetPhysicsManager()->UpdateContactForces(Delta);
	}
	else if (IntegrationMode == 1)
	{

		while (m_gameContext.GetPhysicsManager()->GetRK4StepCount() <= 4)
		{
			// force update
			m_gameContext.GetPhysicsManager()->UpdateForces(Delta);
			// world update (Particles will check current integrate method, and act accordingly)
			m_gameContext.GetWorld()->Update(Delta);
			// physics update
			m_gameContext.GetPhysicsManager()->UpdateContactForces(Delta);
			// Update RK4 step, very important for the particle to know what stage the update is in
			m_gameContext.GetPhysicsManager()->Increment_RK4_step();
		}

		m_gameContext.GetPhysicsManager()->Reset_RK4_Step();
	}


	// render update
	m_gameContext.m_renderer->Update(deltaTime);
}

