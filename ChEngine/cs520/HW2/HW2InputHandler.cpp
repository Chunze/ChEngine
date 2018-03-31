#include "HW2InputHandler.h"
#include "ParticleWorld.h"
#include "Game.h"

HW2InputHandler::HW2InputHandler(GameContext* gameContext)
	: InputHandler(gameContext)
{

}

void HW2InputHandler::ProcessInput(GLFWwindow* contextWindow)
{
	if (glfwGetKey(contextWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(contextWindow, true);
	}

	if (glfwGetMouseButton(contextWindow, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		MOUSE_LEFT_WasPressed = true;
	}

	if (glfwGetMouseButton(contextWindow, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
	{
		if (MOUSE_LEFT_WasPressed)
		{
			MOUSE_LEFT_WasPressed = false;

			//get cursor position
			double xpos, ypos;
			glfwGetCursorPos(contextWindow, &xpos, &ypos);

			int WindowHeight = m_gameContext->GetGame()->WindowHeight;

			ypos = (double)WindowHeight - ypos;
			// create attractor
			static_cast<ParticleWorld*>(m_gameContext->GetWorld())->CreateAttractor(static_cast<int>(xpos), static_cast<int>(ypos));
		}
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
			m_gameContext->GetGame()->bPaused = !m_gameContext->GetGame()->bPaused;
		}
	}
}
