#pragma once
#include "InputHandler.h"

class HW2InputHandler : public InputHandler
{
public:
	HW2InputHandler(GameContext* gameContext);

	virtual void ProcessInput(GLFWwindow* contextWindow) override;

	void GetCursorPosition(GLFWwindow* window, double* x, double* y);
};

