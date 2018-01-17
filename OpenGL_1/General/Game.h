#ifndef GAME_H
#define GAME_H

#include "GameContext.h"

class Game
{
public:
	Game();

	Game(GameContext* gameContext);

	void GameLoop();

	void processInput(GLFWwindow* contextWindow);

	GameContext* m_gameContext;
};

#endif