#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>

class Shader
{
public:

	Shader(const char* vertexShaderFilePath, const char* fragmentShaderFilePath);
	void Use();

	// uniform utility functions
	void SetUniformBool(const std::string &name, bool value) const;
	void SetUniformInt(const std::string &name, int value) const;
	void SetUniformFloat(const std::string &name, float value) const;
	void SetUniformMatrix4(const std::string &name, bool bNeedTranspose, float* value) const;

	unsigned int shaderProgramID;
};

#endif
