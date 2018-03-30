#ifndef RENDERER_H
#define RENDERER_H

#include <iostream>
#include <string>
#include <map>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "BaseClass.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "DrawList.h"

class Light;

struct OnScreenChar
{
	GLuint TextureID;   // ID handle of the glyph texture
	glm::ivec2 Size;    // Size of glyph
	glm::ivec2 Bearing; // Offset from baseline to left/top of glyph
	GLuint Advance;		// Horizontal offset to advance to next glyph
};

class Renderer : public BaseClass
{
public:
	Renderer(GameContext* gameContext);

	void Initialize();

	void InitFreeType();

	void FlyCameraForward(float value);
	void FlyCameraRight(float value);
	void FlyCameraUp(float value);

	void Update(float deltaTime);

	void Draw();
	void SetBackgroundColor(float r, float g, float b);

	void SetActiveCamera(Camera* camera);

	bool bIsFill = true;

	Camera* mainCamera;

	Light* SimpleLight = nullptr;

	void SetLight(Light* LightToSet);

	glm::vec3 cubePositions[10];

	float Background_R = 0.0f;
	float Background_G = 0.0f;
	float Background_B = 0.0f;

	unsigned int OnScreenTextVertexBuffer;				
	unsigned int OnScreenTextVertexArray;
	std::map<GLchar, OnScreenChar> OnScreenChars;
	Shader* OnScreenTextShader;
	void RenderOnScreenText(OnScreenTextElement TextToRender);

	~Renderer()
	{

	}
};

#endif