#include "HW3InputHandler.h"
#include "IKWorld.h"
#include "TailSkel.h"

HW3InputHandler::HW3InputHandler(GameContext* gameContext)
	: InputHandler(gameContext)
{

}

void HW3InputHandler::ProcessInput(GLFWwindow* contextWindow)
{
	InputHandler::ProcessInput(contextWindow);

	if (glfwGetKey(contextWindow, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		KEY_SPACE_WasPressed = true;
	}

	if (glfwGetKey(contextWindow, GLFW_KEY_SPACE) == GLFW_RELEASE)
	{
		if (KEY_SPACE_WasPressed)
		{
			KEY_SPACE_WasPressed = false;
			static_cast<IKWorld*>(m_gameContext->GetWorld())->Tail->TranslateBone(0, glm::vec2(15, 15));
		}
	}

	if (glfwGetMouseButton(contextWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		MOUSE_LEFT_WasPressed = true;
	}

	if (glfwGetMouseButton(contextWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		if (MOUSE_LEFT_WasPressed)
		{
			MOUSE_LEFT_WasPressed = false;

			//get cursor position
			double xpos, ypos;
			GetCursorPosition(contextWindow, &xpos, &ypos);

			static_cast<IKWorld*>(m_gameContext->GetWorld())->Start_IK_Solver_CCD(xpos, ypos, 10, 1);
		}
	}
}

void HW3InputHandler::GetCursorPosition(GLFWwindow* window, double* x, double* y)
{
	glfwGetCursorPos(window, x, y);

	int WindowHeight = m_gameContext->GetGame()->WindowHeight;

	*y = (double)WindowHeight - *y;
}