#include "OverlayShaderProgram.h"

void OverlayShaderProgram::InitUniforms()
{
	float aspect = 1920.0f / 1080.0f;
	u_SetMat4("projection", glm::ortho(-aspect, aspect, 1.0f, -1.0f, 0.0f, -100.0f));
	u_SetMat4("model", glm::identity<glm::mat4>());
	u_SetMat4("view", glm::identity<glm::mat4>());
}

void OverlayShaderProgram::Update(const float& time, const float& dt)
{

}