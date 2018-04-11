#include "InputHandler.h"

void InputHandler::ProcessInput(GLFWwindow* contextWindow)
{
	if (glfwGetKey(contextWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(contextWindow, true);
	}

	if (glfwGetKey(contextWindow, GLFW_KEY_L) == GLFW_PRESS)
	{
		KEY_L_WasPressed = true;
	}
	if (glfwGetKey(contextWindow, GLFW_KEY_L) == GLFW_RELEASE)
	{
		if (KEY_L_WasPressed)
		{
			KEY_L_WasPressed = false;
			m_gameContext->GetGame()->ShowFPS = !m_gameContext->GetGame()->ShowFPS;
		}
	}
}
