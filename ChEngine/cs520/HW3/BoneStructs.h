#ifndef BONE_STRUCTS_H
#define BONE_STRUCTS_H

#include "MathStatics.h"

struct Bone
{
	Bone();
	~Bone()
	{
		delete Parent;
		delete Child;
	}

	float length = 40.0f;

	glm::vec2 Position_World;
	glm::vec2 Direction_Local;
	glm::vec2 Direction_World;

	glm::vec2 Direction_Tangent;

	Bone* Parent = nullptr;
	Bone* Child = nullptr;

	float Orientation = 0.0f;

	glm::mat2x2 RotationMatrix_Local;

	void Setup(float local_orientation = 0.0f);
	glm::vec2 GetEndLocation_World();
	float* UpdateVerticies();

	float vert[15];
};

#endif
