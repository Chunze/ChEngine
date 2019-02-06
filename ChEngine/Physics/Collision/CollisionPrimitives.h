#include "glm.h"

class RigidBody;

enum class PrimitiveType
{
	SPHERE,
	BOX,
};

class PhysicsPrimitive
{
public:
	PrimitiveType GetType() { return m_Type; };

protected:
	RigidBody* m_RigidBody;
	mat4 m_Offset;
	PrimitiveType m_Type;
};

class SpherePrimitive : public PhysicsPrimitive
{
public:
	float m_Radius;
};