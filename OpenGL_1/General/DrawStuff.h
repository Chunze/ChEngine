#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <iostream>
#include <string.h>

using namespace std;

class DrawStuff
{
public:
	DrawStuff();

	void Initialize();

	void InitVertexArray(bool bBindThisVAO);

	void InitVertexBuffer(float* vertices, int size);

	void InitElementBuffer(unsigned int* indices);

	void InitShader(const char* VertexShaderFileName, const char* FragmentShaderFileName);

	string GetShaderFromFile(const char* fileName);

	bool CompileVertexShader(const char* fileName);

	bool CompileFragShader(const char* fileName);

	void CreateShaderProgram(bool bUseThisProgram);

	float* GetVertexData(int &size);

	void DrawTriangle();

	unsigned int vertexShader;		// vertex shader ID
	unsigned int fragmentShader;	// fragment shader ID
	unsigned int shaderProgram;		// shader program ID
	unsigned int VBO;				// vertex buffer ID
	unsigned int VAO;				// vertex array ID
	unsigned int EBO;				// element buffer ID

	bool bVertexShaderCompiled = false;
	bool bFragShaderCompiled = false;

	~DrawStuff()
	{

	}
};