#pragma once
#include "InputHandler.h"

class JelloWorldInputHandler : public InputHandler
{
public:
	JelloWorldInputHandler(GameContext* gameContext);

	virtual void ProcessInput(GLFWwindow* contextWindow) override;
};