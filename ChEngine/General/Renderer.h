#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>
#include <string>

#include "BaseClass.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "DrawList.h"

using namespace std;

class Renderer : public BaseClass
{
public:
	Renderer(GameContext gameContext);

	void Initialize();

	void InitVertexArray(bool bBindThisVAO);

	void InitVertexBuffer(float* vertices, int size);

	void InitElementBuffer(unsigned int* indices);

	void InitLighting();

	void InitShaders();

	float* GetVertexData(int &size);

	void CalculateTransforms();

	void FlyCameraForward(float value);
	void FlyCameraRight(float value);
	void FlyCameraUp(float value);

	void Update(float deltaTime);

	void Draw();
	void InitDrawDebug();
	void CleanupDebugDraw();
	void DrawDebug();

	void JelloRenderModeToggled();

	void TogglePolygonMode();
	bool bIsFill = true;

	unsigned int VBO;				// vertex buffer ID
	unsigned int VAO;				// vertex array ID
	unsigned int EBO;				// element buffer ID
	unsigned int LightVAO;
	unsigned int DebugVertextBuffer;
	unsigned int DebugVertextArray;

	Shader* lampShader;
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

	DrawListElement DebugDrawElement;

	glm::vec3 m_lightPosition;

	glm::vec3 cubePositions[10];

	~Renderer()
	{

	}
};

#endif