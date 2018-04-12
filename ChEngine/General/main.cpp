#include <iostream>

#include "main.h"
#include "Renderer.h"
#include "Game.h"
#include "GameContext.h"

void InitGLFW(int MajorVersion, int MinorVersion, int Profile) {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, MajorVersion);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, MinorVersion);
	glfwWindowHint(GLFW_OPENGL_PROFILE, Profile);

}

int main()
{
	// initialize GLFW
	InitGLFW(3, 3, GLFW_OPENGL_CORE_PROFILE);

	// create window object
	GLFWwindow* window = glfwCreateWindow(640, 480, "ChEngine", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// make the window object to be the main context
	glfwMakeContextCurrent(window);

	// initialize GLAD, which manages function pointers for OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// specify the actual window for the rendering
	glViewport(0, 0, 640, 480);

	// create game context
	GameContext gameContext(window);

	// create game instance
	Game game(&gameContext);
	game.InitGame();

	game.GameLoop();

	// clean up memory
	glfwTerminate();

	return 0;
}

