#include "Renderer.h"

#include "../stb_image.h"

Renderer::Renderer()
{
	Initialize();
}

void Renderer::Initialize()
{
	InitVertexArray(true);

	int size;
	float* vertices = GetVertexData(size);
	InitVertexBuffer(vertices, size);
	InitElementBuffer(NULL);

	simpleShader = new Shader("vertexShader.glsl", "FragmentShader.glsl");
	simpleShader->Use();

	
	texture_0 = new Texture("Textures/container.jpg");

	glActiveTexture(GL_TEXTURE1);
	texture_1 = new Texture("Textures/awesomeface.png", true, true);

	// setting texture unit
	simpleShader->SetUniformInt("texture1", 0);
	simpleShader->SetUniformInt("texture2", 1);
}

void Renderer::InitVertexArray(bool bBindThisVAO)
{
	glGenVertexArrays(1, &VAO);

	if (bBindThisVAO)
	{
		glBindVertexArray(VAO);
	}
}

void Renderer::InitVertexBuffer(float* vertices, int size)
{
	// create vertex buffer object
	glGenBuffers(1, &VBO);

	// bind the VBO to GL_ARRAY_BUFFER target
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// configure data of the target buffer: setting vertices, and draw method
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

	// specify how OpenGL should interpret the vertex buffer data
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}

void Renderer::InitElementBuffer(unsigned int* indices)
{
	unsigned int indices_[] = {
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_), indices_, GL_STATIC_DRAW);

}

float* Renderer::GetVertexData(int &size)
{
	static float vertices[] = {
		// positions          // colors           // texture coords
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
	};

// 	static float vertices[] = {
// 		// positions         // colors
// 		0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
// 		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
// 		0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
// 	};

	size = sizeof(vertices);

	return vertices;
}

void Renderer::Draw()
{
// 	float timeValue = glfwGetTime();
// 	float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
// 
// 	// Setting the uniform variable declared in shader code
// 	simpleShader->SetUniformFloat("greenValue", greenValue);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

// 	glBindVertexArray(VAO);
//	glDrawArrays(GL_TRIANGLES, 0, 3);
}
