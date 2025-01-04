#pragma once
#include "SceneObject.h"
#include "glm.hpp"
#include <ext/matrix_transform.hpp>
class Camera : public SceneObject
{
public:
	Camera():
		m_position(glm::vec3(0.0f, 0.0f, -1.0f)), 
		m_forward(glm::vec3(0.0f, 0.0f, 1.0f)),  
		m_up(glm::vec3(0.0f, 1.0f, 0.0f)) {
		updateTransform();
	};

	void SetPosition(glm::vec3& position);
	void SetForward(glm::vec3& forward);
	void SetUp(glm::vec3& up);

	glm::vec3& GetPosition();
	glm::vec3& GetForward();
	glm::vec3& GetUp();

	void Init();
	void Update();
	void Start();
	glm::mat4& GetTransform();
private:
	void updateTransform();
	glm::vec3 m_position;
	glm::vec3 m_forward;
	glm::vec3 m_up;
};

