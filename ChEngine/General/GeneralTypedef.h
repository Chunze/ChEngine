#ifndef GENERAL_TYPEDEF_H
#define GENERAL_TYPEDEF_H
#pragma once
#include <memory>
#include <vector>
#include <queue>

class IPhysicsProxy;
class IRenderableObject;
class Component;

typedef std::shared_ptr<Component> Component_sp;

typedef std::vector<IPhysicsProxy*> PhysicsProxies;
typedef std::vector<IRenderableObject*> RenderableObjects;
typedef std::vector<Component_sp> Components;

#endif
