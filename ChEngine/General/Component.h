#pragma once
#include "SceneObject.h"

class GameObject;

class Component : public SceneObject
{
	typedef SceneObject Super;
public:
	Component(GameContext* gameContext);
	Component(GameContext* gameContext, World* world);

	void AddChild(Component_sp child) { m_Children.push_back(child); }
	void RemoveChild(Component_sp child);

	/** getters **/
	Components GetChildren() { return m_Children; }
	GameObject* GetOwner() { return m_Owner; }
	bool IsRoot() { return bIsRoot; }

	/** setters **/
	void SetOwner(GameObject* owner);
	void SetIsRoot(bool IsRoot) { bIsRoot = IsRoot; }

	/** SceneObject functions **/
	virtual void Update(float Delta) override;
	virtual void UpdateTransform() override;
	/** SceneObject functions **/

	~Component();

protected:
	GameObject* m_Owner = nullptr;

	Component_sp m_Parent = nullptr;

	Components m_Children;

	// This indicates whether this SceneObject is the root of some GameObject
	bool bIsRoot = false;

};

