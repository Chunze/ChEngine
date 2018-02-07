#include "Renderer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "../stb_image.h"

#include "DrawList.h"

using namespace glm;

Renderer::Renderer(GameContext gameContext)
	: BaseClass(gameContext)
{
	Initialize();
}

void Renderer::Initialize()
{
	m_lightPosition = vec3(0.0f, 6.0f, 0.0f);

	InitVertexArray(true);

	float* vertices = GetVertexData(vertextBufferSize);
	InitVertexBuffer(vertices, vertextBufferSize);
	InitElementBuffer(NULL);

	InitShaders();

	num_vertex = vertextBufferSize / (vertexInfoSize * sizeof(float));

	InitDrawDebug();

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

	vertexInfoSize = 8;

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexInfoSize * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexInfoSize * sizeof(float), (void*)(3 * sizeof(float)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertexInfoSize * sizeof(float), (void*)(6 * sizeof(float)));

	

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

void Renderer::InitLighting()
{
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// we only need to bind to the VBO, the container's VBO's data already contains the correct data.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// set the vertex attributes (only position data for our lamp)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void Renderer::InitShaders()
{
	/*
	simpleShader = new Shader("vertexShader.glsl", "FragmentShader.glsl");
	simpleShader->Use();

	texture_0 = new Texture("Textures/container.jpg");

	glActiveTexture(GL_TEXTURE1);
	texture_1 = new Texture("Textures/awesomeface.png", true, true);

	// setting texture unit
	simpleShader->SetUniformInt("texture1", 0);
	simpleShader->SetUniformInt("texture2", 1);
	*/


	simpleShader = new Shader("vertexShader.glsl", "LightingShader.frag");
	simpleShader->Use();
	//simpleShader->SetUniformVector("objectColor", 1.0f, 0.5f, 0.31f);
	//simpleShader->SetUniformVector("material.ambient", 1.0f, 0.5f, 0.31f);
	//simpleShader->SetUniformVector("material.diffuse", 1.0f, 0.5f, 0.31f);
	simpleShader->SetUniformVector("material.specular", 0.5f, 0.5f, 0.5f);
	simpleShader->SetUniformFloat("material.shininess", 32.0f);
	//simpleShader->SetUniformVector("lightColor", 1.0f);
	simpleShader->SetUniformVector("light.ambient", 0.2f, 0.2f, 0.2f);
	simpleShader->SetUniformVector("light.diffuse", 0.5f, 0.5f, 0.5f); // darken the light a bit to fit the scene
	simpleShader->SetUniformVector("light.specular", 1.0f, 1.0f, 1.0f);
	simpleShader->SetUniformVector("light.position", value_ptr(m_lightPosition));

	glActiveTexture(GL_TEXTURE0);
	texture_0 = new Texture("Textures/container2.png");
	glActiveTexture(GL_TEXTURE1);
	texture_1 = new Texture("Textures/container2_specular.png");

	simpleShader->SetUniformInt("material.diffuse", 0);
	simpleShader->SetUniformInt("material.specular", 1);

	lampShader = new Shader("vertexShader.glsl", "LampShader.frag");
}

float* Renderer::GetVertexData(int &size)
{
	static float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
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

	// TODO: update the transform in c++ instead of in shader
	simpleShader->SetUniformMatrix4("model", false, value_ptr(model));
	simpleShader->SetUniformMatrix4("view", false, value_ptr(view));
	simpleShader->SetUniformMatrix4("projection", false, value_ptr(projection));
	simpleShader->SetUniformVector("viewPos", value_ptr(mainCamera->m_position));

	lampShader->SetUniformMatrix4("model", false, value_ptr(model));
	lampShader->SetUniformMatrix4("view", false, value_ptr(view));
	lampShader->SetUniformMatrix4("projection", false, value_ptr(projection));

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

void Renderer::Update(float deltaTime)
{
	//CalculateTransforms();

	glEnable(GL_DEPTH_TEST);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mainCamera->Update(deltaTime);

	Draw();

	//DrawDebug();
	//CleanupDebugDraw();
}

void Renderer::Draw()
{
	/*		camera		*/
	glm::mat4 view;
	view = mainCamera->m_view;

	/*		NDC			*/
	glm::mat4 projection;
	projection = perspective(radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);


	DrawList drawList = *m_gameContext.GetDrawList();

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(0.0f,  3.0f, 5.0f),
		glm::vec3(0.0f, 6.0f, 0.0f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};


	glm::mat4 model;
	model = glm::translate(model, cubePositions[1]);

	// draw drawlist
	for (auto drawCall : drawList.m_elements)
	{
		drawCall.GetRenderReady();
		drawCall.shader.SetUniformMatrix4("model", false, value_ptr(model));
		drawCall.shader.SetUniformMatrix4("view", false, value_ptr(view));
		drawCall.shader.SetUniformMatrix4("projection", false, value_ptr(projection));
		drawCall.shader.SetUniformVector("viewPos", value_ptr(mainCamera->m_position));
		drawCall.shader.SetUniformVector("material.specular", 0.5f, 0.5f, 0.5f);
		drawCall.shader.SetUniformFloat("material.shininess", 32.0f);
		//simpleShader->SetUniformVector("lightColor", 1.0f);
		drawCall.shader.SetUniformVector("light.ambient", 0.2f, 0.2f, 0.2f);
		drawCall.shader.SetUniformVector("light.diffuse", 0.5f, 0.5f, 0.5f); // darken the light a bit to fit the scene
		drawCall.shader.SetUniformVector("light.specular", 1.0f, 1.0f, 1.0f);
		drawCall.shader.SetUniformVector("light.position", value_ptr(m_lightPosition));

		GLenum drawingMode = (GLenum)drawCall.drawingPrimitive;

		glDrawArrays(drawingMode, 0, drawCall.numOfVertices);
		drawCall.DisableAttributePointer();
	}

	//simpleShader->SetUniformMatrix4("model", false, value_ptr(model));
	
	// TODO: keep track of how many attributes, and clean up accordingly 
// 	glEnableVertexAttribArray(0);
// 	glEnableVertexAttribArray(1);
// 	glEnableVertexAttribArray(2);
// 	glBindBuffer(GL_ARRAY_BUFFER, VBO);
// 	glBindVertexArray(VAO);
// 	simpleShader->Use();
// 	glDrawArrays(GL_TRIANGLES, 0, num_vertex);
// 
// 	model = glm::mat4();
// 	model = glm::translate(model, cubePositions[2]);
// 	lampShader->Use();
// 	lampShader->SetUniformMatrix4("model", false, value_ptr(model));
// 	glDrawArrays(GL_TRIANGLES, 0, num_vertex);
}

void Renderer::CleanupDraw()
{
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void Renderer::InitDrawDebug()
{
	
	glGenVertexArrays(1, &DebugVertextArray);
	glBindVertexArray(DebugVertextArray);
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

	//glBufferData(GL_ARRAY_BUFFER, sizeof(axisLineVertices), axisLineVertices, GL_STATIC_DRAW);
	debugShader = new Shader("DebugDrawShader.vert", "DebugDrawShader.frag");
	
	DrawListElement e;
	e.vertexBuffer = axisLineVertices;
	e.drawingPrimitive = DrawingPrimitives::LINES;
	e.shader = *debugShader;
	e.VBsize = sizeof(axisLineVertices);
	e.attributeSizes.push_back(3);
	e.attributeSizes.push_back(3);
	e.vertextInfoSize = 6;

	m_gameContext.GetDrawList()->Add(e, -1);

	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
}

void Renderer::CleanupDebugDraw()
{
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void Renderer::DrawDebug()
{
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, DebugVertextBuffer);
	glBindVertexArray(DebugVertextArray);
	debugShader->Use();

	glDrawArrays(GL_LINES, 0, 6);
}
