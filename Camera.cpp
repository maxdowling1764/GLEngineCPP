#include "Camera.h"

void Camera::Update(float dt)
{
	m_position += m_velocity * dt;
	m_view = glm::lookAt(m_position, m_position + m_orientation.a, m_orientation.b);
}

void Camera::SetPosition(glm::vec3 pos)
{
	m_position = pos;
}

void Camera::SetOrientation(glm::vec3 forward, glm::vec3 up)
{
	m_orientation = Bivector(forward, up);
}

void Camera::UpdateProjection()
{
	m_proj = glm::perspective(m_fov, m_aspect, m_zNear, m_zFar);
}


