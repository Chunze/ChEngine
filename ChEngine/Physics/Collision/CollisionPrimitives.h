#ifndef COLLISION_PRIMITIVES_H
#define COLLISION_PRIMITIVES_H

#include "glm.h"
#include "PhysicsTypes.h"

class RigidBody;

enum class PrimitiveType
{
	SPHERE,
	BOX,
	SURFACE,
	NUM_OF_PRIMITIVE_TYPE	// not a real type, convenience for max number of primitive types
};

class CollisionPrimitive
{
public:

	/**    setters    **/
	void SetBody(RigidBody_sp Body, const mat4 &Offset = mat4()) { m_RigidBody = Body; }

	/**    getters    **/
	PrimitiveType GetType() { return m_Type; }
	vec3 GetPosition();
	RigidBody_sp GetBody() { return m_RigidBody; }

protected:
	RigidBody_sp m_RigidBody = nullptr;
	mat4 m_Offset;
	PrimitiveType m_Type;
	
};

class SurfasePrimitive : public CollisionPrimitive
{
public:
	SurfasePrimitive() { m_Type = PrimitiveType::SURFACE; }
	SurfasePrimitive(vec3 Normal, float Offset)
		: m_Offset(Offset)
	{
		m_Normal = glm::normalize(Normal);
		m_Type = PrimitiveType::SURFACE;
	}

	vec3 m_Normal;
	float m_Offset;
};

class SpherePrimitive : public CollisionPrimitive
{
public:
	SpherePrimitive() { m_Type = PrimitiveType::SPHERE; }
	SpherePrimitive(float Radius)
		: m_Radius(Radius)
	{
		m_Type = PrimitiveType::SPHERE;
	}
	float m_Radius;
};

class BoxPrimitive : public CollisionPrimitive
{
	BoxPrimitive() { m_Type = PrimitiveType::BOX; }
};

#endif // !COLLISION_PRIMITIVES_H
