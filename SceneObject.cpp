#include "SceneObject.h"

void SceneObject::Init()
{
	for (SceneObject* child : m_children)
	{
		if (child != nullptr)
		{
			child->Init();
		}
	}
};

void SceneObject::Update()
{
	if (m_parent != nullptr)
		m_transform = m_parent->m_transform * m_model;

	for (SceneObject* child : m_children)
	{
		if (child != nullptr)
		{
			child->Update();
		}
	}
};

SceneObject::~SceneObject()
{
	if (m_parent != nullptr)
	{
		m_parent->m_children.erase(this);
	}
	for (SceneObject* child : m_children)
	{
		child->m_parent = nullptr;
	}
};

void SceneObject::SetParent(SceneObject* parent)
{
	m_parent = parent;
	parent->m_children.insert(this);
}

void SceneObject::SetTransform(glm::mat4& transform) { m_transform = transform; }
const glm::mat4& SceneObject::GetTransform() { return m_transform; }

