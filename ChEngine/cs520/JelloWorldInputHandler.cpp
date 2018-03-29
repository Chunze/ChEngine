#include "JelloWorldInputHandler.h"
#include "Game.h"
#include "JelloWorld.h"

GameContext* _gameContext;
double _mouselastX = 400, _mouselastY = 300;
bool _firstMouse = true;

void _mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (_firstMouse) // this bool variable is initially set to true
	{
		_mouselastX = xpos;
		_mouselastY = ypos;
		_firstMouse = false;
	}

	float offsetX = (float)(xpos - _mouselastX);
	float offsetY = (float)(_mouselastY - ypos);
	_mouselastX = xpos;
	_mouselastY = ypos;

	float sensitivity = 0.05f;
	offsetX *= sensitivity;
	offsetY *= sensitivity;

	_gameContext->m_renderer->mainCamera->Rotate(offsetY, offsetX);
}

JelloWorldInputHandler::JelloWorldInputHandler(GameContext* gameContext)
	: InputHandler(gameContext)
{
	_gameContext = gameContext;
}

void JelloWorldInputHandler::ProcessInput(GLFWwindow* contextWindow)
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
			m_gameContext->GetGame()->bPaused = !m_gameContext->GetGame()->bPaused;
		}
	}

	if (glfwGetMouseButton(contextWindow, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		glfwSetInputMode(contextWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetCursorPosCallback(contextWindow, _mouse_callback);

		if (glfwGetKey(contextWindow, GLFW_KEY_W) == GLFW_PRESS)
		{
			m_gameContext->m_renderer->FlyCameraForward(1.0f);
		}
		if (glfwGetKey(contextWindow, GLFW_KEY_A) == GLFW_PRESS)
		{
			m_gameContext->m_renderer->FlyCameraRight(-1.0f);
		}
		if (glfwGetKey(contextWindow, GLFW_KEY_S) == GLFW_PRESS)
		{
			m_gameContext->m_renderer->FlyCameraForward(-1.0f);
		}
		if (glfwGetKey(contextWindow, GLFW_KEY_D) == GLFW_PRESS)
		{
			m_gameContext->m_renderer->FlyCameraRight(1.0f);
		}
		if (glfwGetKey(contextWindow, GLFW_KEY_Q) == GLFW_PRESS)
		{
			m_gameContext->m_renderer->FlyCameraUp(-1.0f);
		}
		if (glfwGetKey(contextWindow, GLFW_KEY_E) == GLFW_PRESS)
		{
			m_gameContext->m_renderer->FlyCameraUp(1.0f);
		}
	}

	if (glfwGetMouseButton(contextWindow, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
	{
		// mouse right release, show cursor, unbind callback function and reset
		glfwSetInputMode(contextWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		glfwSetCursorPosCallback(contextWindow, NULL);
		_firstMouse = true;
	}

	if (glfwGetKey(contextWindow, GLFW_KEY_F4) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	if (glfwGetKey(contextWindow, GLFW_KEY_F5) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
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
			JelloWorld* _jelloWorld = static_cast<JelloWorld*>(m_gameContext->GetWorld());
			if (_jelloWorld)
			{
				_jelloWorld->ToggleBendSpring();
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
			JelloWorld* _jelloWorld = static_cast<JelloWorld*>(m_gameContext->GetWorld());
			if (_jelloWorld)
			{
				_jelloWorld->ToggleShearSpring();
			}
		}
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

	if (glfwGetKey(contextWindow, GLFW_KEY_V) == GLFW_PRESS)
	{
		KEY_V_WasPressed = true;
	}
	if (glfwGetKey(contextWindow, GLFW_KEY_V) == GLFW_RELEASE)
	{
		if (KEY_V_WasPressed)
		{
			KEY_V_WasPressed = false;
			JelloWorld* _jelloWorld = static_cast<JelloWorld*>(m_gameContext->GetWorld());
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
			JelloWorld* _jelloWorld = static_cast<JelloWorld*>(m_gameContext->GetWorld());
			if (_jelloWorld)
			{
				_jelloWorld->ToggleStructuralSpring();
			}
		}
	}
}

