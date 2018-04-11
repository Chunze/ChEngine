#include "TailSkel.h"

TailSkel::TailSkel(GameContext* gameContext, World* world, int NumofBones/* = 1*/)
	: GameObject(gameContext, world)
{
	VertexBuffer = new float[NumofBones * Bone::VertexBufferSize];
	Structure = *(new std::vector<Bone>(NumofBones));
	InitBones(NumofBones);
	InitDrawElement();
}

void TailSkel::CreateAndAddDrawListElement(int Mode)
{

}

void TailSkel::Update(float Delta)
{
	UpdateVertexBuffer();
	m_gameContext->GetDrawList()->AddToDrawQ(e, false);
}

void TailSkel::InitBones(int NumOfBones /*= 1*/)
{
	Structure.clear();

// 	int Bones = NumOfBones;
// 	while (Bones > 0)
// 	{
// 		Bone* NewBone = new Bone();
// 		Structure.push_back(*NewBone);
// 		Bones--;
// 	}

	for (int index = 0; index < NumOfBones; index++)
	{
		Bone* NewBone = new Bone();
		Structure.push_back(*NewBone);

		if (index == 0)
		{
			Structure[0].bIsRoot = true;
		}

		if (index > 0)
		{
			Structure[index - 1].Child = &(Structure[index]);
			Structure[index].Parent = &(Structure[index - 1]);
		}
	}

	for (auto& bone : Structure)
	{
		bone.Setup();
	}
}

void TailSkel::InitDrawElement()
{
	e.attributeSizes.push_back(2);
	e.attributeSizes.push_back(3);
	e.drawingPrimitive = DrawingPrimitives::TRIANGLES;
	e.shader = *(new Shader("ScreenSpaceParticle.vert", "ScreenSpaceParticle.frag"));
	e.vertextInfoSize = 5;
	e.VBsize_inByte = Structure.size() * Bone::VertexBufferSize * 4;
	UpdateVertexBuffer();
	e.vertexBuffer = VertexBuffer;
}

void TailSkel::SetupBones(glm::vec2 Root_Position, std::vector<float> AnglesFromRoot)
{
	assert(Structure.size() <= 0);

	Structure[0].Position_World = Root_Position;

	int NumOfBones = Structure.size();
	for (size_t i = 0; i <= AnglesFromRoot.size(); i++)
	{
		if (i <= NumOfBones)
		{
			Structure[i].Setup(AnglesFromRoot[i]);
		}
	}
}

void TailSkel::RotateBone(size_t index, float degree)
{
	if (index > Structure.size())
	{
		return;
	}

	Structure[index].RotateBone(degree);
}

void TailSkel::TranslateBone(size_t index, glm::vec2 translation)
{
	if (index > Structure.size())
	{
		return;
	}

	Structure[index].TranslateBone(translation);
}

void TailSkel::UpdateVertexBuffer()
{
	for (size_t i = 0; i < Structure.size(); i++)
	{
		auto bone = Structure[i];
		float* VertexInfo = bone.UpdateVerticies();
		memcpy(VertexBuffer + Bone::VertexBufferSize * i, VertexInfo, Bone::VertexBufferSize * sizeof(float));
	}
}

// float* TailSkel::GetBoneVerticies()
// {
// 
// }
