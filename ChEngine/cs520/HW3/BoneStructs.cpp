#include "BoneStructs.h"

Bone::Bone()
{
	Orientation = 0.0f;
	RotationMatrix_Local = Math::Get2DRotationMat(Orientation);
	Direction_Local = glm::vec2(1.0f, 0.0f) * RotationMatrix_Local;
	Direction_World = glm::vec2(1.0f, 0.0f);
	Direction_Tangent = glm::vec2(0.0f, 1.0f);
	Position_World = glm::vec2(0.0f, 100.0f);
}

void Bone::Setup(float local_orientation/* = 0.0f*/)
{
	Orientation = local_orientation;
	RotationMatrix_Local = Math::Get2DRotationMat(local_orientation);
	Direction_Local = glm::vec2(1.0f, 0.0f) * RotationMatrix_Local;

	if (Parent != nullptr)
	{
		Direction_World = Parent->Direction_World * RotationMatrix_Local;
		Direction_Tangent = Direction_World * Math::Get2DRotationMat(90.0f);
		glm::normalize(Direction_World);
		glm::normalize(Direction_Tangent);

		Position_World = Parent->GetEndLocation_World();
	}
}

glm::vec2 Bone::GetEndLocation_World()
{
	return Position_World + length * Direction_World;
}

float* Bone::UpdateVerticies()
{
	// point 1
	vert[0] = GetEndLocation_World().x;
	vert[1] = GetEndLocation_World().y;

	vert[2] = 1.0f;
	vert[3] = 1.0f;
	vert[4] = 1.0f;

	// point 2
	vert[5] = (Position_World + Direction_Tangent * 10.0f).x;
	vert[6] = (Position_World + Direction_Tangent * 10.0f).y;

	vert[7] = 1.0f;
	vert[8] = 1.0f;
	vert[9] = 1.0f;

	// point 3
	vert[10] = (Position_World - Direction_Tangent * 10.0f).x;
	vert[11] = (Position_World - Direction_Tangent * 10.0f).y;

	vert[12] = 1.0f;
	vert[13] = 1.0f;
	vert[14] = 1.0f;

	return vert;
}
