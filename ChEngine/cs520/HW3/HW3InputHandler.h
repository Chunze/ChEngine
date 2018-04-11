#pragma once

#include "InputHandler.h"

class HW3InputHandler : public InputHandler
{
public:
	HW3InputHandler(GameContext* gameContext);

	virtual void ProcessInput(GLFWwindow* contextWindow) override;
};