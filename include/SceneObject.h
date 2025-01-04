#pragma once
#include <set>
#include <glm.hpp>
class SceneObject
{
protected:
	SceneObject* m_parent;
	glm::mat4 m_transform;
	glm::mat4 m_model;
	std::set<SceneObject*> m_children;

public:
	SceneObject() 
		: m_transform(glm::mat4(1.0f)), 
		m_model(glm::mat4(1.0f)), 
		m_children({}), 
		m_parent(nullptr) {};

	SceneObject(glm::mat4 model) 
		: m_transform(model),
		m_model(model),
		m_children({}), 
		m_parent(nullptr) {};

	SceneObject(glm::mat4 model, SceneObject* parent) 
		: m_model(model), 
		m_children({}), 
		m_parent(parent) 
	{
		m_transform = (m_parent == nullptr) ? m_parent->GetTransform() * m_model : m_model;
	};

	void SetTransform(glm::mat4& transform);
	const glm::mat4& GetTransform();
	virtual void Update();

	// Called on first render call, regardless of whether the scene object is active.
	// Derived classes should call SceneObject::Init() at the very end of the overriding method by convention.
	virtual void Init();

	virtual ~SceneObject();
	void SetParent(SceneObject* parent);
};

