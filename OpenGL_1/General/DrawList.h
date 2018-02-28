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
	size_t VBsize;
	// TODO: add vector for attribute pointer info

	size_t vertextInfoSize;
	size_t numOfVertices;
	std::vector<int> attributeSizes;

	Shader shader;
	// buffers
	unsigned int vertexBufferObject;
	unsigned int vertexArrayObject;

	bool bIsDebug = false;

	void GetRenderReady();
	void DisableAttributePointer();
};


class DrawList : public BaseClass
{
public:
	DrawList(GameContext gameContext);

	std::vector<DrawListElement> m_elements;

	int Add(DrawListElement elementToAdd, int index);
	
	void Clear();
	bool IsHaveNewData() { return bHasNewData; };

	// will be overloaded
	void UpdateElement(int index, float* newVB, int VBsize_byte);

protected:

	bool bHasNewData;
};