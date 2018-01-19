#ifndef GAME_H
#define GAME_H

#include "GameContext.h"

class Game
{
public:
	Game(GameContext* gameContext);

	void GameLoop();

	void processInput(GLFWwindow* contextWindow);

	GameContext* m_gameContext;

	float deltaTime = 0.0f;
	float lastFrameTime = 0.0f;

};

#endif