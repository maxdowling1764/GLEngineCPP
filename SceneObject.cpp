#include "SceneObject.h"

void SceneObject::SetTransform(glm::mat4& transform) { m_transform = transform; }
glm::mat4& SceneObject::GetTransform() { return m_transform; }