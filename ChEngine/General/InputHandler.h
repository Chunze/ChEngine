#pragma once
#include "BaseClass.h"
#include "Renderer.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class InputHandler : public BaseClass
{

public:
	InputHandler(GameContext* gameContext)
		: BaseClass(gameContext)
	{}

	virtual void ProcessInput(GLFWwindow* contextWindow) = 0;

protected:

	bool KEY_B_WasPressed = false;
	bool KEY_H_WasPressed = false;
	bool KEY_L_WasPressed = false;
	bool KEY_V_WasPressed = false;
	bool KEY_X_WasPressed = false;


	bool KEY_SPACE_WasPressed = false;
};