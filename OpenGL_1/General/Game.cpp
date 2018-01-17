
#include "Game.h"
#include "Renderer.h"

Game::Game()
{

}

Game::Game(GameContext* gameContext)
	: m_gameContext(gameContext)
{

}

void Game::GameLoop()
{
	GLFWwindow* contextWindow = m_gameContext->m_contextWindow;

	while (!glfwWindowShouldClose(contextWindow))
	{
		// input
		processInput(contextWindow);

		// render update
		m_gameContext->renderer->Update();
		
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
	if (glfwGetKey(contextWindow, GLFW_KEY_W) == GLFW_PRESS)
	{
		m_gameContext->renderer->FlyCameraForward(1.0f);
	}
	if (glfwGetKey(contextWindow, GLFW_KEY_A) == GLFW_PRESS)
	{
		m_gameContext->renderer->FlyCameraRight(-1.0f);
	}
	if (glfwGetKey(contextWindow, GLFW_KEY_S) == GLFW_PRESS)
	{
		m_gameContext->renderer->FlyCameraForward(-1.0f);
	}
	if (glfwGetKey(contextWindow, GLFW_KEY_D) == GLFW_PRESS)
	{
		m_gameContext->renderer->FlyCameraRight(1.0f);
	}
	if (glfwGetKey(contextWindow, GLFW_KEY_Q) == GLFW_PRESS)
	{
		m_gameContext->renderer->FlyCameraUp(-1.0f);
	}
	if (glfwGetKey(contextWindow, GLFW_KEY_E) == GLFW_PRESS)
	{
		m_gameContext->renderer->FlyCameraUp(1.0f);
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