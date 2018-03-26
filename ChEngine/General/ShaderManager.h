#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

#include <glad/glad.h>

class ShaderManager
{
public:
	ShaderManager();

	bool LoadAndCompileVertexShader(const char* fileName);
	bool LoadAndCompileFragShader(const char* fileName);
	void CreateShaderProgram(bool bUseThisProgram);


	unsigned int vertexShader;		// vertex shader ID
	unsigned int fragmentShader;	// fragment shader ID
	unsigned int shaderProgram;		// shader program ID
};

#endif // SHADER_MANAGER_H