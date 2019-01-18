#pragma once
#include "PrimitiveComponent.h"
#include "Model.h"

class StaticMeshComponent : public PrimitiveComponent
{
	typedef PrimitiveComponent Super;
public:
	StaticMeshComponent(GameContext* gameObject, World* world);

	void AddDrawListElement() override;

	void SetMesh(Model* model) { m_Model = model; }
	
	Model* m_Model;

	~StaticMeshComponent();
};

