#ifndef GAME_CONTEXT_H
#define GAME_CONTEXT_H

struct GLFWwindow;
class Renderer;
class PhysicsManager;
class World;
class DrawList;
class Game;
class InputHandler;

class GameContext
{
public:
	GameContext(GLFWwindow* window);

	World* m_world;
	GLFWwindow* m_contextWindow;
	Renderer* m_renderer;
	PhysicsManager* m_physicsManager;
	DrawList* m_drawList;
	Game* m_Game;
	InputHandler* m_InputHandler;

	// getters
	inline Renderer* GetRenderer() { return m_renderer; }
	inline GLFWwindow* GetWindow() { return m_contextWindow; }
	inline PhysicsManager* GetPhysicsManager() { return m_physicsManager; }
	inline World* GetWorld() { return m_world; }
	inline DrawList* GetDrawList() { return m_drawList; }
	inline Game* GetGame() { return m_Game; }
	inline InputHandler* GetInputHandler() { return m_InputHandler; }
};

#endif