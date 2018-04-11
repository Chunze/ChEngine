#ifndef TAIL_SKELETON_H
#define TAIL_SKELETON_H

#include <vector>

#include "GameObject.h"
#include "BoneStructs.h"

class TailSkel : public GameObject
{
public:
	TailSkel(GameContext* gameContext, World* world, int NumofBones = 1);

	virtual void CreateAndAddDrawListElement(int Mode = 0) override;
	virtual void Update(float Delta) override;

	void InitBones(int NumOfBones = 1);
	void InitDrawElement();
	void SetupBones(glm::vec2 Root_Position, std::vector<float> AnglesFromRoot);
/*	float* GetBoneVerticies();*/

	std::vector<Bone> Structure;

	void RotateBone(size_t index, float degree);

	void TranslateBone(size_t index, glm::vec2 translation);

	void UpdateVertexBuffer();

	float* VertexBuffer;
};

#endif
