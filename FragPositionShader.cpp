#include "FragPositionShader.h"

void FragPositionShader::InitUniforms()
{
	// Define your field of view, aspect ratio, near plane, and far plane
	float fov = glm::radians(90.0f); // Field of view angle in radians
	float aspectRatio = 1920.0f / 1080.0f; // Aspect ratio of your viewport
	float nearPlane = 0.1f; // Near clipping plane distance
	float farPlane = 100.0f; // Far clipping plane distance

	// Construct the perspective projection matrix

	u_SetMat4("projection", glm::perspective(fov, aspectRatio, nearPlane, farPlane));
	u_SetMat4("model", glm::identity<glm::mat4>());
	Update(0.0f, 0.001f);
}

void FragPositionShader::Update(const float& time, const float& dt)
{
	u_SetMat4("view", p_cam->GetTransform());
}