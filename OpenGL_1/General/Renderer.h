#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

#include "Shader.h"
#include "Texture.h"

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

	void Draw();

	unsigned int VBO;				// vertex buffer ID
	unsigned int VAO;				// vertex array ID
	unsigned int EBO;				// element buffer ID

	Shader* simpleShader;
	Texture* simpleTexture;

	bool bVertexShaderCompiled = false;
	bool bFragShaderCompiled = false;

	~Renderer()
	{

	}
};