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

	float deltaTime = 0.0f;
	float lastFrameTime = 0.0f;

};

#endif