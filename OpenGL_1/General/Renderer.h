#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>
#include <string>

#include "Shader.h"
#include "Texture.h"
#include "Camera.h"

using namespace std;

class Renderer
{
public:
	Renderer();

	void Initialize();

	void InitVertexArray(bool bBindThisVAO);

	void InitVertexBuffer(float* vertices, int size);

	void InitElementBuffer(unsigned int* indices);

	float* GetVertexData(int &size);

	void CalculateTransforms();

	void FlyCameraForward(float value);
	void FlyCameraRight(float value);
	void FlyCameraUp(float value);

	void Update();

	void Draw();

	void SetupDraw();

	void CleanupDraw();

	void SetupDrawDebug();

	void CleanupDebugDraw();

	void DrawDebug();

	unsigned int VBO;				// vertex buffer ID
	unsigned int VAO;				// vertex array ID
	unsigned int EBO;				// element buffer ID
	unsigned int DebugVertextBuffer;
	unsigned int DebugVertextArray;

	Shader* simpleShader;
	Shader* debugShader;
	Texture* texture_0;
	Texture* texture_1;

	Camera* mainCamera;

	bool bVertexShaderCompiled = false;
	bool bFragShaderCompiled = false;

	int vertextBufferSize;
	int vertexInfoSize;

	int num_vertex;

	glm::vec3 cubePositions[10];

	~Renderer()
	{

	}
};

#endif