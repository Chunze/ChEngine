#include <iostream>

#include "Game.h"

#include "Jello.h"

// components and managers for the game
#include "Renderer.h"
#include "PhysicsManager.h"
#include "JelloWorld.h"

GameContext* gameContext;
float mouselastX = 400, mouselastY = 300;
bool firstMouse = true;

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse) // this bool variable is initially set to true
	{
		mouselastX = xpos;
		mouselastY = ypos;
		firstMouse = false;
	}

	float offsetX = xpos - mouselastX;
	float offsetY = mouselastY - ypos;
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
	// creating components and managers for the game
	{
		DrawList* drawList = new DrawList(m_gameContext);
		m_gameContext.m_drawList = drawList;
	}
	{
		Renderer* renderer = new Renderer(m_gameContext);
		m_gameContext.m_renderer = renderer;
	}
	{
		PhysicsManager* physicsManager = new PhysicsManager(m_gameContext);
		m_gameContext.m_physicsManager = physicsManager;
	}
	{
		World* world = new JelloWorld(m_gameContext);
		world->LoadWorld("cs520/jello.w");
		m_gameContext.m_world = world;
	}
}

void Game::GameLoop()
{
	GLFWwindow* contextWindow = m_gameContext.m_contextWindow;
	gameContext = &m_gameContext;
	

	while (!glfwWindowShouldClose(contextWindow))
	{
		bWasPausedLastFrame = bPaused;
		float currentTime = glfwGetTime();
		deltaTime = currentTime - lastFrameTime;
		lastFrameTime = currentTime;

		// input
		processInput(contextWindow);


		if (bWasPausedLastFrame && !bPaused)
		{
			deltaTime = 0.0f;
			bWasPausedLastFrame = false;
		}


		deltaTime = 0.0005;
		Update(deltaTime);
		
		
		

		//std::cout << "Frame: " << deltaTime << endl;
		
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
		bPaused = false;
	}

	if (glfwGetKey(contextWindow, GLFW_KEY_ENTER) == GLFW_PRESS)
	{
		bPaused = true;
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
}

void Game::Update(float Delta)
{
	int IntegrationMode = m_gameContext.GetPhysicsManager()->GetIntegrator();
	if (!bGamePaused || !bPaused)
	{
		if (IntegrationMode == 0)
		{
			// force update
			m_gameContext.GetPhysicsManager()->UpdateForces(Delta);

			// world update
			m_gameContext.GetWorld()->Update(Delta);

			// TODO: need to call generate contact info here to separate world update so that RK4 would not do this 4 times

			// physics update

			m_gameContext.GetPhysicsManager()->UpdateContactForces(Delta);
		}
		else if (IntegrationMode == 1)
		{
			//TODO: make a copy to all particle in the world (save this as the original copy of last frame)


			while (m_gameContext.GetPhysicsManager()->GetRK4StepCount() <= 4)
			{
				// force update
				m_gameContext.GetPhysicsManager()->UpdateForces(Delta);
				// world update (Particles will check current integrate method, and act accordingly)
				m_gameContext.GetWorld()->Update(Delta);

				// Update RK4 step, very important for the particle to know what stage the update is in
				m_gameContext.GetPhysicsManager()->Increment_RK4_step();
			}

			m_gameContext.GetPhysicsManager()->Reset_RK4_Step();

			// TODO: Update all the particles using original copy of last frame

			// TODO: Generate contact info

			// TODO: resolve contacts (need to take care of this in RK4)
		}

		bGamePaused = true;
	}

	// render update
	m_gameContext.m_renderer->Update(deltaTime);
}

