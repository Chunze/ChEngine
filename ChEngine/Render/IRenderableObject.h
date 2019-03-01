#ifndef RENDERABLE_H
#define RENDERABLE_H
#pragma once
#include <vector>

#include "DrawList.h"

class IRenderableObject
{
public:
	virtual void GetherDrawCall(DrawList* drawList) const = 0;
};

#endif
