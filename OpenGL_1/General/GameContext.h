#ifndef GAME_CONTEXT_H
#define GAME_CONTEXT_H

struct GLFWwindow;
class Renderer;

class GameContext
{
public:
	GameContext(GLFWwindow* window);

	GLFWwindow* m_contextWindow;
	Renderer* renderer;
};

#endif