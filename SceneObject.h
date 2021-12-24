#pragma once
#include <vector>
#include <glm/glm.hpp>
class SceneObject
{
protected:
	SceneObject* m_parent;
	glm::mat4 m_transform;
	
public:
	std::vector<SceneObject*> m_children;

	SceneObject() :m_transform(glm::mat4(1.0f)), m_children({}),m_parent(nullptr) {};
	SceneObject(glm::mat4 transform) :m_transform(transform), m_children({}), m_parent(nullptr) {};
	SceneObject(glm::mat4 transform, SceneObject* parent) :m_transform(transform), m_children({}), m_parent(parent) {};
	void Update();
	void Init();
	void Start();
};

