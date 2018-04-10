#include "TailSkel.h"

TailSkel::TailSkel(GameContext* gameContext, World* world, int NumofBones/* = 1*/)
	: GameObject(gameContext, world)
{
	InitBones(NumofBones);
	InitDrawElement();
}

void TailSkel::CreateAndAddDrawListElement(int Mode)
{

}

void TailSkel::Update(float Delta)
{
	m_gameContext->GetDrawList()->AddToDrawQ(e, false);
}

void TailSkel::InitBones(int NumOfBones /*= 1*/)
{
	Structure.clear();

	int Bones = NumOfBones;
	while (Bones > 0)
	{
		Bone* NewBone = new Bone();
		Structure.push_back(*NewBone);
		Bones--;
	}
}

void TailSkel::InitDrawElement()
{
	e.attributeSizes.push_back(2);
	e.attributeSizes.push_back(3);
	e.drawingPrimitive = DrawingPrimitives::TRIANGLES;
	e.shader = *(new Shader("ScreenSpaceParticle.vert", "ScreenSpaceParticle.frag"));
	e.vertextInfoSize = 5;
	Structure[0].UpdateVerticies();
	e.VBsize_inByte = 15 * 4;
	e.vertexBuffer = Structure[0].vert;
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

// float* TailSkel::GetBoneVerticies()
// {
// 
// }
