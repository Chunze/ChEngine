#include "ShaderManager.h"
#include "FileManager.h"

#include <iostream>

ShaderManager::ShaderManager()
{

}

bool ShaderManager::LoadAndCompileVertexShader(const char* fileName)
{
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	std::string VShader_s = FileManager::ReadTextFileAsString(fileName);
	const char* VertexShaderSource = VShader_s.c_str();

	// load shader source
	glShaderSource(vertexShader, 1, (const GLchar**)&VertexShaderSource, NULL);

	// compile the loaded shader
	glCompileShader(vertexShader);

	// checking compilation
	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;

		return false;
	}

	return true;
}

bool ShaderManager::LoadAndCompileFragShader(const char* fileName)
{
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	std::string FShader_s = FileManager::ReadTextFileAsString(fileName);
	const char* FShader = FShader_s.c_str();

	glShaderSource(fragmentShader, 1, &FShader, NULL);
	glCompileShader(fragmentShader);

	// checking compilation
	int  success;
	char infoLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;

		return false;
	}

	return true;
}

void ShaderManager::CreateShaderProgram(bool bUseThisProgram)
{
	// create shader program
	shaderProgram = glCreateProgram();

	// link the compiled shader to the program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// check linking result
	int  success;
	char infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::LINKING::LINKING_FAILED\n" << infoLog << std::endl;
		return;
	}

	if (bUseThisProgram)
	{
		// activate the program
		glUseProgram(shaderProgram);

		// clean up the memory
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}
}
