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
}

