#include "Camera.h"

void Camera::Init()
{

}

void Camera::Update()
{
}

void Camera::Start()
{

}

void Camera::SetPosition(glm::vec3& position)
{
	m_position = position;
	updateTransform();
}


void Camera::SetForward(glm::vec3& forward) {
	m_forward = forward;
	updateTransform();
}

void Camera::SetUp(glm::vec3& up)
{
	m_up = up;
	updateTransform();
}

glm::vec3& Camera::GetPosition()
{
	return m_position;
}

glm::vec3& Camera::GetForward()
{
	return m_forward;
}

glm::vec3& Camera::GetUp()
{
	return m_up;
}
glm::mat4& Camera::GetTransform()
{
	return m_transform;
}

void Camera::updateTransform()
{
	glm::mat4 t = glm::lookAt(m_position, m_position + m_forward, m_up);
	
	this->SetTransform(t);
}