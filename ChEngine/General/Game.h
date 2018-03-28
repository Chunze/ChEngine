#ifndef GAME_H
#define GAME_H

#include "BaseClass.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Game : public BaseClass
{
public:
	Game(GameContext* gameContext);

	void InitGame();

	void GameLoop();

	void processInput(GLFWwindow* contextWindow);

	void saveScreenshot(int windowWidth, int windowHeight, char *filename);

	virtual void Update(float Delta);

	float deltaTime = 0.0f;
	float lastFrameTime = 0.0f;
	float lastScreenCapture = 0.0f;
	int ScreenCaptureIndex = 0;

	bool bWasPausedLastFrame = false;
	bool bPaused = true;
	bool bGamePaused = false;

	int WindowWidth;
	int WindowHeight;
	float FPS;

	bool ShowFPS = false;
private:

	bool KEY_B_WasPressed = false;
	bool KEY_H_WasPressed = false;
	bool KEY_L_WasPressed = false;
	bool KEY_V_WasPressed = false;
	bool KEY_X_WasPressed = false;
	
	
	bool KEY_SPACE_WasPressed = false;
};

#endif