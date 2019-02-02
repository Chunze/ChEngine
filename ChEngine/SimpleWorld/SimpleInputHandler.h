#pragma once
#include "InputHandler.h"

class SimpleInputHandler : public InputHandler
{
public:
	SimpleInputHandler(GameContext* gameContext);

	virtual void ProcessInput(GLFWwindow* contextWindow) override;
};