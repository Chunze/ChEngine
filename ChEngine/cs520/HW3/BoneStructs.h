#ifndef BONE_STRUCTS_H
#define BONE_STRUCTS_H

#include "MathStatics.h"

struct Bone
{
	Bone();

	float length = 50.0f;

	glm::vec2 Position_World;
	glm::vec2 Direction_Local;
	glm::vec2 Direction_World;

	glm::vec2 Direction_Tangent;

	bool bIsRoot = false;

	Bone* Parent = nullptr;
	Bone* Child = nullptr;

	float Orientation = 0.0f;

	glm::mat2x2 RotationMatrix_Local;

	void Setup(float local_orientation = 0.0f);
	glm::vec2 GetEndLocation_World();
	float* UpdateVerticies();

	void TranslateBone(glm::vec2 translation);

	void RotateBone(float degree);

	static const int VertexBufferSize = 15;
};

#endif
