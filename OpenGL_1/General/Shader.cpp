#include "Shader.h"

#include "FileManager.h"



Shader::Shader(const char* vertexShaderFilePath, const char* fragmentShaderFilePath)
{
	// -loading vertex shader-
	std::string VShader_s = FileManager::ReadTextFileAsString(vertexShaderFilePath);
	const char* vertexShaderSource = VShader_s.c_str();

	// loading fragment shader
	std::string FShader_s = FileManager::ReadTextFileAsString(fragmentShaderFilePath);
	const char* fragmentShaderSource = FShader_s.c_str();

	// -compiling the shaders-
	unsigned int vertexShader, fragmentShader;
	int success;
	char infoLog[512];
	
	// - vertex shader -	
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// load shader source
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);

	// compile the loaded shader
	glCompileShader(vertexShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// - fragment shader -	
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// create shader program
	shaderProgramID = glCreateProgram();

	// link the compiled shader to the program
	glAttachShader(shaderProgramID, vertexShader);
	glAttachShader(shaderProgramID, fragmentShader);
	glLinkProgram(shaderProgramID);

	// check linking result
	glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgramID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::LINKING::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::Use()
{
	glUseProgram(shaderProgramID);
}

void Shader::CheckCurrentProgram()
{
	GLint currentProgramID;
	glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgramID);
	if (shaderProgramID != currentProgramID)
	{
		Use();
	}
}

void Shader::SetUniformBool(const std::string &name, bool value)
{
	CheckCurrentProgram();
	glUniform1i(glGetUniformLocation(shaderProgramID, name.c_str()), (int)value);
}
void Shader::SetUniformInt(const std::string &name, int value)
{
	CheckCurrentProgram();
	glUniform1i(glGetUniformLocation(shaderProgramID, name.c_str()), value);
}
void Shader::SetUniformFloat(const std::string &name, float value)
{
	CheckCurrentProgram();
	glUniform1f(glGetUniformLocation(shaderProgramID, name.c_str()), value);
}

void Shader::SetUniformMatrix4(const std::string &name, bool bNeedTranspose, float* value)
{
	CheckCurrentProgram();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, name.c_str()), 1, bNeedTranspose, value);
}
