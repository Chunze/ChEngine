#ifndef SHAPE_COMPONENT_H
#define SHAPE_COMPONENT_H
#pragma once
#include "IRenderableObject.h"
#include "PrimitiveComponent.h"

class ShapeComponent : public PrimitiveComponent, public IRenderableObject
{
	typedef PrimitiveComponent Super;

public:
	ShapeComponent(GameContext *gameContext, World* world);

};

#endif
