#ifndef PHYSICS_TYPES_H
#define PHYSICS_TYPES_H
#pragma once
#include <memory>
#include <vector>

class Particle;
class RigidBody;
class CollisionPrimitive;

typedef std::shared_ptr<Particle> Particle_sp;
typedef std::shared_ptr<RigidBody> RigidBody_sp;
typedef std::shared_ptr<CollisionPrimitive> CollisionPrimitive_sp;


typedef std::vector<CollisionPrimitive_sp> CollisionPrimitives;

#endif // !PHYSICS_TYPES_H
