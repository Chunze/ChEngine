#ifndef GAME_H
#define GAME_H

#include "BaseClass.h"

class Game : public BaseClass
{
public:
	Game(GameContext* gameContext);

	void InitGame();

	void GameLoop();

	void processInput(GLFWwindow* contextWindow);

	virtual void Update(float Delta);

	float deltaTime = 0.0f;
	float lastFrameTime = 0.0f;

	bool bWasPausedLastFrame = false;
	bool bPaused = true;
	bool bGamePaused = false;

private:
	bool KEY_V_WasPressed = false;
	bool KEY_S_WasPressed = false;
	bool KEY_H_WasPressed = false;
	bool KEY_B_WasPressed = false;
	bool KEY_SPACE_WasPressed = false;
};

#endif