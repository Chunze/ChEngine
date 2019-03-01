#ifndef STATIC_MESH_COMPONENT_H
#define STATIC_MESH_COMPONENT_H
#pragma once
#include "Model.h"
#include "IRenderableObject.h"
#include "PrimitiveComponent.h"

class StaticMeshComponent : public PrimitiveComponent, public IRenderableObject
{
	typedef PrimitiveComponent Super;
public:
	StaticMeshComponent(GameContext* gameObject, World* world);

	virtual void GetherDrawCall(DrawList* drawList) const;

	void SetMesh(Model* model) { m_Model = model; }
	
	Model* m_Model;

	~StaticMeshComponent();

private:
	void InitPhysicsParticle();
	void InitPhysicsBody();

	bool bUseCollision = true;
};

#endif
