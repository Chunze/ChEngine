
class RigidBody;

enum class PrimitiveType
{
	SPHERE,
	BOX,
};

class PhysicsPrimitive
{
protected:
	PrimitiveType m_Type;
};