#include "CollisionDetection.h"
#include "CollisionTestUtils.h"
#include "PhysicsManager.h"

#include <iostream>

float CollisionDetection::m_CoherencePenetrationLimit = 0.1f;

CollisionDetection::CollisionDetection(PhysicsManager* PhysicsManager)
	: m_PhysicsManager(PhysicsManager),
	  m_BroadPhaseTest(this),
	  m_NarrowPhaseTest(this)
{
	
}

void CollisionDetection::RunCollisionDetection(CollisionInfo *Data)
{
	m_BroadPhaseTest.RunTest();

	m_NarrowPhaseTest.RunTest(Data);

	GenerateCoherenceContact(Data);
}

void CollisionDetection::RegisterCollisionPrimitive(CollisionPrimitive_sp PrimitiveToAdd)
{
	if (!PrimitiveToAdd->GetBody())
	{
		for (auto primitive : m_PrimitiveWithoutBody)
		{
			if (primitive == PrimitiveToAdd)
			{
				return;
			}
		}
		m_PrimitiveWithoutBody.push_back(PrimitiveToAdd);
	}
}


void CollisionDetection::GenerateCoherenceContact(CollisionInfo *Data)
{
	if(Data->m_ContactFeatureList.m_ContactFeatures.size() > 0)
		std::cout << "Contact Feature size: " << Data->m_ContactFeatureList.m_ContactFeatures.size() << '\n';

	for (ContactFeature &Feature : Data->m_ContactFeatureList.m_ContactFeatures)
	{
		if (Feature.IsValid())
		{
			// This feature was not added this frame, generate a contact from it
			BoxPrimitive* Box1 = static_cast<BoxPrimitive*>(Feature.m_Primitives[0]);
			BoxPrimitive* Box2 = static_cast<BoxPrimitive*>(Feature.m_Primitives[1]);
			BodyContact Contact;
			float Penetration;
			vec3 Axis = Box1->GetAxis(Feature.m_FeatureID[0]);

			if (Feature.m_FeatureType == ContactFeatureType::FACE_VS_VERTEX)
			{
				Penetration = CollisionTestUtils::BoxPenetrationOnAxis(Box1, Box2, Axis);

				if (Penetration < -m_CoherencePenetrationLimit)
				{
					continue;
				}

				Contact.m_Penetration = Penetration;
				
				CollisionTestUtils::GetContactInfoFaceVsVertex(Box1, Box2, Axis, Contact);

				Data->AddContact(Contact, false);
			}
			else if (Feature.m_FeatureType == ContactFeatureType::EDGE_VS_EDGE)
			{
				vec3 Axis = glm::cross(Box1->GetAxis(Feature.m_FeatureID[0]), Box2->GetAxis(Feature.m_FeatureID[1]));
				Penetration = CollisionTestUtils::BoxPenetrationOnAxis(Box1, Box2, Axis);

				if (Penetration < -m_CoherencePenetrationLimit)
				{
					continue;
				}

				Contact.m_Penetration = Penetration;

				CollisionTestUtils::GetContactInfoEdgeVsEdge(Box1, Box2,
					Feature.m_FeatureID[0],
					Feature.m_FeatureID[1],
					Contact);

				Data->AddContact(Contact, false);
			}
		}

		Feature.m_FramesLived++;
	}

	Data->CleanupFeatureList();
}
