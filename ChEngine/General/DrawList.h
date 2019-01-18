#pragma once
#include <vector>
#include <queue>
#include "BaseClass.h"
#include "Shader.h"
#include "Texture.h"

enum class DrawingPrimitives
{
	POINTS			= 0x0000,
	LINES			= 0x0001,
	LINE_STRIP,
	LINE_LOOP,
	TRIANGLES,
	TRIANGLE_STRIP,
	TRIANGLE_FAN,
	QUADS,
	QUAD_STRIP,
	POLYGON
};

enum class DrawingMode
{
	DRAW_ARRAY,
	DRAW_ELEMENT
};

struct DrawListElement
{
	DrawListElement() {}

	DrawingPrimitives drawingPrimitive;
	DrawingMode drawingMode;

	void* vertexBuffer;
	unsigned int* indexBuffer;
	size_t VBsize_inByte;
	size_t IBsize_inByte;
	size_t IBsize;

	size_t vertextInfoSize;
	size_t numOfVertices;
	std::vector<int> attributeSizes;

	Shader shader;
	std::vector<Texture> textures;


	// buffers
	unsigned int vertexBufferObject;
	unsigned int indexBufferObject;
	unsigned int vertexArrayObject;
	float PointSize = 5.0f;
	float LineWidth = 1.0f;
	bool bIsDebug = false;

	// scene
	glm::mat4 worldTransform;

	// texture
	bool bHasTexture = false;
	std::string TextureDir;

	void Init();
	void GetRenderReady();
	void DisableAttributePointer();
};

struct OnScreenTextElement
{
	OnScreenTextElement() {}

	OnScreenTextElement(std::string textToRender, float X, float Y, float Scale, glm::vec3 Color)
		: Text(textToRender),
		x(X),
		y(Y),
		scale(Scale),
		color(Color)
	{}

	Shader shader;
	std::string Text;
	float x;		// on screen position x
	float y;		// on screen position y
	float scale;	// font size
	glm::vec3 color;
};


class DrawList : public BaseClass
{
public:
	DrawList(GameContext* gameContext);

	// This vector gets cleared each frame
	std::queue<DrawListElement> m_DynamicElements;

	// This vector changes much less frequent
	std::queue<DrawListElement> m_StaticElements;

	std::queue<OnScreenTextElement> m_OnScreenTexts;

	void AddToDrawQ(DrawListElement& elementToAdd, bool bIsDynamic = true);

	void AddToDrawQ(std::vector<DrawListElement> elements, bool bIsDynamic = true);

	void AddOnScreenText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
	
	void Clear();
	bool IsHaveNewData() { return bHasNewData; };

protected:

	bool bHasNewData;
};