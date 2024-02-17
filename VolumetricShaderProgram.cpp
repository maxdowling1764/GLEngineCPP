#include "VolumetricShaderProgram.h"

void VolumetricShaderProgram::InitUniforms()
{
	u_SetVec3("cam_pos", p_cam->GetPosition());
	u_SetVec3("cam_forward", p_cam->GetForward());
	u_SetVec3("cam_up", p_cam->GetUp());
	u_SetMat4("projection", glm::ortho(-1.0f, 1.0f, 1.0f, -1.0f, 0.0f, -100.0f));
}

void VolumetricShaderProgram::Update(const float& time, const float& dt)
{
	u_SetFloat("time", time);
	u_SetVec3("cam_forward", p_cam->GetForward());
	u_SetVec3("cam_up", p_cam->GetUp());
	u_SetVec3("cam_pos", p_cam->GetPosition());
}