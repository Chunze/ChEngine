#include "Renderer.h"

#include <iostream>

#include "Game.h"
#include "../stb_image.h"

const float BoundaryLineColor = 0.7f;

Renderer::Renderer(GameContext* gameContext)
	: BaseClass(gameContext)
{
	Initialize();
}

void Renderer::Initialize()
{
	m_lightPosition = glm::vec3(0.0f, 6.0f, 0.0f);

	InitFreeType();
	glEnable(GL_DEPTH_TEST);
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

void Renderer::InitFreeType()
{
	OnScreenTextShader = new Shader("OnScreenText.vert", "OnScreenText.frag");

	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(m_gameContext->GetGame()->WindowWidth), 0.0f, static_cast<float>(m_gameContext->GetGame()->WindowHeight));
	OnScreenTextShader->SetUniformMatrix4("projection", false, glm::value_ptr(projection));

	FT_Library ft;
	if (FT_Init_FreeType(&ft))
	{
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
	}

	FT_Face face;
	if (FT_New_Face(ft, "Fonts/COURIER.TTF", 0, &face))
	{
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
	}

	// Set size to load glyphs as
	FT_Set_Pixel_Sizes(face, 0, 24);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

	for (GLubyte c = 0; c < 128; c++)
	{
		// Load character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}
		// Generate texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Now store character for later use
		OnScreenChar character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		OnScreenChars.insert(std::pair<GLchar, OnScreenChar>(c, character));
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	// Destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	glGenVertexArrays(1, &OnScreenTextVertexArray);
	glGenBuffers(1, &OnScreenTextVertexBuffer);
	glBindVertexArray(OnScreenTextVertexArray);
	glBindBuffer(GL_ARRAY_BUFFER, OnScreenTextVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
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
	projection = mainCamera->m_perpective;

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

	

	glClearColor(Background_R, Background_G, Background_B, 1.0f);
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
	projection = mainCamera->m_perpective;

	DrawList* drawQueue = m_gameContext->GetDrawList();

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));

	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);

	while (!drawQueue->m_DynamicElements.empty())
	{
		auto drawCall = drawQueue->m_DynamicElements.front();
		drawCall.GetRenderReady();
		glPointSize(drawCall.PointSize);
		glLineWidth(drawCall.LineWidth);
		{
			glm::mat4 temp;
			model = glm::translate(temp, cubePositions[0]);
			//model = glm::rotate(model, 70.f, glm::vec3(1, 2, 3));
			drawCall.shader.SetUniformMatrix4("model", false, value_ptr(model));
		}

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

		drawQueue->m_DynamicElements.pop();
	}

	while (!drawQueue->m_StaticElements.empty())
	{
		auto drawCall = drawQueue->m_StaticElements.front();

		drawCall.GetRenderReady();
		glPointSize(drawCall.PointSize);
		glLineWidth(drawCall.LineWidth);
		glm::mat4 temp;
		model = glm::translate(temp, cubePositions[0]);
		drawCall.shader.SetUniformMatrix4("model", false, value_ptr(model));
		drawCall.shader.SetUniformMatrix4("view", false, value_ptr(view));
		drawCall.shader.SetUniformMatrix4("projection", false, value_ptr(projection));
		drawCall.shader.SetUniformVector("viewPos", value_ptr(mainCamera->m_position));

		GLenum drawingMode = (GLenum)drawCall.drawingPrimitive;

		glDrawArrays(drawingMode, 0, drawCall.numOfVertices);
		drawCall.DisableAttributePointer();

		drawQueue->m_StaticElements.pop();
	}

	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	while (!drawQueue->m_OnScreenTexts.empty())
	{
		auto textToRender = drawQueue->m_OnScreenTexts.front();
		RenderOnScreenText(textToRender);
		drawQueue->m_OnScreenTexts.pop();
	}

}

void Renderer::SetBackgroundColor(float r, float g, float b)
{
	Background_R = r;
	Background_G = g;
	Background_B = b;
}

void Renderer::SetActiveCamera(Camera* camera)
{
	mainCamera = camera;
}

void Renderer::RenderOnScreenText(OnScreenTextElement TextToRender)
{
	glm::vec3 color = TextToRender.color;
	std::string text = TextToRender.Text;
	float x = TextToRender.x;
	float y = TextToRender.y;
	float scale = TextToRender.scale;

	OnScreenTextShader->SetUniformVector("textColor", color.x, color.y, color.z);
	OnScreenTextShader->Use();

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(OnScreenTextVertexArray);

	// Iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		OnScreenChar ch = OnScreenChars[*c];

		GLfloat xpos = x + ch.Bearing.x * scale;
		GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		GLfloat w = ch.Size.x * scale;
		GLfloat h = ch.Size.y * scale;
		// Update VBO for each character
		GLfloat vertices[6][4] = {
			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos,     ypos,       0.0, 1.0 },
			{ xpos + w, ypos,       1.0, 1.0 },

			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos + w, ypos,       1.0, 1.0 },
			{ xpos + w, ypos + h,   1.0, 0.0 }
		};
		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, OnScreenTextVertexBuffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
