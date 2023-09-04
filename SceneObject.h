#pragma once
#include <vector>
#include <glm.hpp>
class SceneObject
{
protected:
	SceneObject* m_parent;
	glm::mat4 m_transform;
	std::vector<SceneObject*> m_children;

public:
	SceneObject() :m_transform(glm::mat4(1.0f)), m_children({}),m_parent(nullptr) {};
	virtual ~SceneObject() {};
	SceneObject(glm::mat4 transform) :m_transform(transform), m_children({}), m_parent(nullptr) {};
	SceneObject(glm::mat4 transform, SceneObject* parent) :m_transform(transform), m_children({}), m_parent(parent) {};

	void SetTransform(glm::mat4& transform);
	glm::mat4& GetTransform();
	virtual void Update() {};		// Called at least once each render call
	virtual void Init() {};		// Called during construction
	virtual void Start() {};		// Called on first render call
};

