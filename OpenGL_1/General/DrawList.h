#pragma once
#include <vector>

#include "BaseClass.h"

enum class DrawingPrimitives
{
	POINTS,
	LINES,
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
};

class DrawList : public BaseClass
{
public:
	DrawList(GameContext gameContext);

	std::vector<DrawListElement> elements;

	void Add(DrawListElement elementToAdd);
	void Clear();
	bool IsHaveNewData() { return bHasNewData; };

protected:

	bool bHasNewData;
};