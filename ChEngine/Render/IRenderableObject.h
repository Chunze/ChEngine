#ifndef RENDERABLE_H
#define RENDERABLE_H
#pragma once
#include "DrawList.h"

class IRenderableObject
{
public:
	virtual std::vector<DrawListElement> GetherDrawCall() const = 0;
};

#endif
