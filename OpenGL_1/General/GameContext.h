#ifndef GAME_CONTEXT_H
#define GAME_CONTEXT_H

struct GLFWwindow;
class Renderer;
class PhysicsManager;
class World;

class GameContext
{
public:
	GameContext(GLFWwindow* window);

	World* m_world;
	GLFWwindow* m_contextWindow;
	Renderer* m_renderer;
	PhysicsManager* m_physicsManager;

	// getters
	inline Renderer* GetRenderer() { return m_renderer; }
	inline GLFWwindow* GetWindow() { return m_contextWindow; }
	inline PhysicsManager* GetPhysicsManager() { return m_physicsManager; }
	inline World* GetWorld() { return m_world; }
};

#endif