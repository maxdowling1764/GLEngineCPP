#pragma once
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
struct Bivector
{
	glm::vec3 a, b;
	glm::vec3 n;

	Bivector(glm::vec3 x, glm::vec3 y)
	{
		a = x;
		b = y;
		n = glm::cross(a, b);
	}
};
class Camera
{
private:
	// These get updated within Update() based on public members
	glm::mat4 m_view;
	glm::mat4 m_proj;
public:
	glm::vec3 m_position;
	glm::vec3 m_velocity;
	Bivector m_orientation;
	float m_zNear, m_zFar;
	float m_fov;
	float m_aspect;

	/// <summary>
	/// 
	/// </summary>
	/// <param name="forward"></param>
	/// <param name="up"></param>
	/// <param name="zn"></param>
	/// <param name="zf"></param>
	/// <param name="aspect"></param>
	/// <param name="fov"></param>
	Camera(glm::vec3 forward, glm::vec3 up, float zn, float zf, float aspect, float fov):
		m_position(glm::vec3(0.0f)),
		m_orientation(Bivector(forward, up)),
		m_velocity(glm::vec3(0.0f)),
		m_zNear(zn),
		m_zFar(zf),
		m_aspect(aspect),
		m_fov(fov)
	{
		m_view = glm::lookAt(m_position, m_position + m_orientation.a, m_orientation.b);
		m_proj = glm::perspective(m_fov, m_aspect, m_zNear, m_zFar);
	}
	void Update(float dt);
	void SetPosition(glm::vec3 pos);
	void SetOrientation(glm::vec3 forward, glm::vec3 up);
	void UpdateProjection();
	
	glm::mat4 GetViewMatrix() { return m_view; };
	glm::mat4 GetProjectionMatrix() { return m_proj; };

};

