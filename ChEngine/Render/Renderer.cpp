#include "Renderer.h"

#include <iostream>

#include "Game.h"
#include "Light.h"
#include "../stb_image.h"

const float BoundaryLineColor = 0.7f;

Renderer::Renderer(GameContext* gameContext)
	: BaseClass(gameContext)
{
	Initialize();
}

void Renderer::Initialize()
{
	InitFreeType();
	glEnable(GL_DEPTH_TEST);
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
	glClearColor(Background_R, Background_G, Background_B, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mainCamera->Update(deltaTime);

	Draw();

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

		if (SimpleLight != nullptr)
		{
			glm::vec3 position = SimpleLight->m_position;
			glm::vec3 ambient = SimpleLight->m_ambient;
			glm::vec3 diffuse = SimpleLight->m_diffuse;
			glm::vec3 specular = SimpleLight->m_specular;
			drawCall.shader.SetUniformVector("light.ambient", ambient.x, ambient.y, ambient.z);
			drawCall.shader.SetUniformVector("light.diffuse", diffuse.x, diffuse.y, diffuse.z); // darken the light a bit to fit the scene
			drawCall.shader.SetUniformVector("light.specular", specular.x, specular.y, specular.z);
			drawCall.shader.SetUniformVector("light.position", value_ptr(position));
		}

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

void Renderer::SetLight(Light* LightToSet)
{
	SimpleLight = LightToSet;
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
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

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
