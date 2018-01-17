#include "Renderer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../stb_image.h"

using namespace glm;

Renderer::Renderer()
{
	Initialize();
}

void Renderer::Initialize()
{
	InitVertexArray(true);

	float* vertices = GetVertexData(vertextBufferSize);
	InitVertexBuffer(vertices, vertextBufferSize);
	InitElementBuffer(NULL);

	simpleShader = new Shader("vertexShader.glsl", "FragmentShader.glsl");
	simpleShader->Use();

	
	texture_0 = new Texture("Textures/container.jpg");

	glActiveTexture(GL_TEXTURE1);
	texture_1 = new Texture("Textures/awesomeface.png", true, true);

	// setting texture unit
	simpleShader->SetUniformInt("texture1", 0);
	simpleShader->SetUniformInt("texture2", 1);

	num_vertex = vertextBufferSize / (vertexInfoSize * sizeof(float));

	SetupDrawDebug();

	mainCamera = new Camera();
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	vertexInfoSize = 5;

// 	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
// 	glEnableVertexAttribArray(2);
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
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	size = sizeof(vertices);

	return vertices;
}

void Renderer::CalculateTransforms()
{
	/*		Model		*/
	glm::mat4 model;

	/*		camera		*/
	glm::mat4 view;
	view = mainCamera->m_view;

	/*		NDC			*/
	glm::mat4 projection;
	projection = perspective(radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);

	simpleShader->SetUniformMatrix4("model", false, value_ptr(model));
	simpleShader->SetUniformMatrix4("view", false, value_ptr(view));
	simpleShader->SetUniformMatrix4("projection", false, value_ptr(projection));

	debugShader->SetUniformMatrix4("model", false, value_ptr(model));
	debugShader->SetUniformMatrix4("view", false, value_ptr(view));
	debugShader->SetUniformMatrix4("projection", false, value_ptr(projection));
}

void Renderer::FlyCameraForward(float value)
{
	mainCamera->FlyCameraForward(value);
}

void Renderer::FlyCameraRight(float value)
{
	mainCamera->FlyCameraRight(value);
}

void Renderer::FlyCameraUp(float value)
{
	mainCamera->FlyCameraUp(value);
}

void Renderer::Update()
{
	CalculateTransforms();

	glEnable(GL_DEPTH_TEST);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mainCamera->Update();

	//SetupDraw();
	Draw();
	//CleanupDraw();

	//SetupDrawDebug();
	DrawDebug();
	//CleanupDebugDraw();
}

void Renderer::Draw()
{
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};


	glm::mat4 model;
	model = glm::translate(model, cubePositions[3]);
	float angle = 20.0f * 3;
	model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
	simpleShader->SetUniformMatrix4("model", false, value_ptr(model));
	

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindVertexArray(VAO);
	simpleShader->Use();

	glDrawArrays(GL_TRIANGLES, 0, num_vertex);
}

void Renderer::SetupDraw()
{

}

void Renderer::CleanupDraw()
{
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void Renderer::SetupDrawDebug()
{
	glGenBuffers(1, &DebugVertextBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, DebugVertextBuffer);
	glLineWidth(3.0f);
	float axisLineVertices[] = {
		0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		5.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 5.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 5.0f, 0.0f, 0.0f, 1.0f,
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(axisLineVertices), axisLineVertices, GL_STATIC_DRAW);
	debugShader = new Shader("DebugDrawShader.vert", "DebugDrawShader.frag");
	
	glGenVertexArrays(1, &DebugVertextArray);
	glBindVertexArray(DebugVertextArray);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
}

void Renderer::CleanupDebugDraw()
{
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void Renderer::DrawDebug()
{
	glBindBuffer(GL_ARRAY_BUFFER, DebugVertextBuffer);
	glBindVertexArray(DebugVertextArray);
	debugShader->Use();

	glDrawArrays(GL_LINES, 0, 6);
}
