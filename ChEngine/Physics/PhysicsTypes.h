#ifndef PHYSICS_TYPES_H
#define PHYSICS_TYPES_H
#pragma once
#include <memory>
#include <vector>
#include <queue>

class CollisionPrimitive;
class Particle;
class ParticleContact;
class RigidBody;

struct PotentialBodyContact;
struct CollisionInfo;

/**    smart pointers    **/
typedef std::shared_ptr<Particle> Particle_sp;
typedef std::shared_ptr<RigidBody> RigidBody_sp;
typedef std::shared_ptr<CollisionPrimitive> CollisionPrimitive_sp;
typedef std::shared_ptr<ParticleContact> ParticleContact_sp;
typedef std::shared_ptr<PotentialBodyContact> PotentialBodyContact_sp;

typedef std::vector<CollisionPrimitive_sp> CollisionPrimitives;
typedef std::vector<Particle_sp> Particles;
typedef std::vector<ParticleContact_sp> ParticleContacts;
typedef std::vector<RigidBody_sp> RigidBodies;

typedef std::queue<PotentialBodyContact> PotentialBodyContacts;
typedef std::queue<CollisionInfo> CollisionInfos;

#endif // !PHYSICS_TYPES_H
