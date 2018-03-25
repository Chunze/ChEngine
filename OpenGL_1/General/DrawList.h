#pragma once
#include <vector>

#include "BaseClass.h"
#include "Shader.h"


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

struct DrawListElement
{
	DrawingPrimitives drawingPrimitive;

	float* vertexBuffer;
	size_t VBsize_inByte;

	size_t vertextInfoSize;
	size_t numOfVertices;
	std::vector<int> attributeSizes;

	Shader shader;
	// buffers
	unsigned int vertexBufferObject;
	unsigned int vertexArrayObject;
	float PointSize = 5.0f;
	float LineWidth = 1.0f;
	bool bIsDebug = false;

	void GetRenderReady();
	void DisableAttributePointer();
};


class DrawList : public BaseClass
{
public:
	DrawList(GameContext gameContext);

	// This vector gets cleared each frame
	std::vector<DrawListElement> m_DynamicElements;

	// This vector changes much less frequent
	std::vector<DrawListElement> m_StaticElements;

	int Add(DrawListElement elementToAdd, int index, bool bIsDynamic = true);
	
	void Clear();
	bool IsHaveNewData() { return bHasNewData; };

	// will be overloaded
	void UpdateElement(int index, float* newVB, int VBsize_byte);

protected:

	bool bHasNewData;
};