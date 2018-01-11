#include "DrawStuff.h"

DrawStuff::DrawStuff()
{
	Initialize();
}

void DrawStuff::Initialize()
{
	InitVertexArray(true);

	int size;
	float* vertices = GetVertexData(size);
	InitVertexBuffer(vertices, size);
	InitElementBuffer(NULL);
	InitShader("vertexShader.glsl", "FragmentShader.glsl");
}

void DrawStuff::InitVertexArray(bool bBindThisVAO)
{
	glGenVertexArrays(1, &VAO);

	if (bBindThisVAO)
	{
		glBindVertexArray(VAO);
	}
}

void DrawStuff::InitVertexBuffer(float* vertices, int size)
{
	// create vertex buffer object
	glGenBuffers(1, &VBO);

	// bind the VBO to GL_ARRAY_BUFFER target
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// configure data of the target buffer: setting vertices, and draw method
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

	// specify how OpenGL should interpret the vertex buffer data
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}

void DrawStuff::InitElementBuffer(unsigned int* indices)
{
	unsigned int indices_[] = {
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_), indices_, GL_STATIC_DRAW);

}

void DrawStuff::InitShader(const char* VertexShaderFileName, const char* FragmentShaderFileName)
{
	if (!CompileVertexShader("vertexShader.glsl"))
	{
		return;
	}

	if (!CompileFragShader("FragmentShader.glsl"))
	{
		return;
	}

	CreateShaderProgram(true);
}

std::string DrawStuff::GetShaderFromFile(const char* fileName)
{
	ifstream ShaderFile(fileName);

	if (!ShaderFile.good())
	{
		std::cout << "ERROR::SHADER::LOADING::Fail to load shader from file" << fileName << std::endl;
		return "";
	}

	return std::string(
		std::istreambuf_iterator<char>(ShaderFile),
		std::istreambuf_iterator<char>()
	);
}

bool DrawStuff::CompileVertexShader(const char* fileName)
{
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	string VShader_s = GetShaderFromFile(fileName);
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

	bVertexShaderCompiled = true;
	return true;
}

bool DrawStuff::CompileFragShader(const char* fileName)
{
	
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	string FShader_s = GetShaderFromFile(fileName);
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

	bFragShaderCompiled = true;
	return true;
}

void DrawStuff::CreateShaderProgram(bool bUseThisProgram)
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

float* DrawStuff::GetVertexData(int &size)
{
	static float vertices[] = {
		0.5f,  0.5f, 0.0f,  // top right
		0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left 
	};

	size = sizeof(vertices);

	return vertices;
}

void DrawStuff::DrawTriangle()
{
	glUseProgram(shaderProgram);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	//glBindVertexArray(VAO);
	//glDrawArrays(GL_TRIANGLES, 0, 3);
}
