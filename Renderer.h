#pragma once
#include <vector>
#include "VolumeElement.h"
#include "ShaderProgram.h"
#include "SceneObject.h"
#include "glm.hpp"
class Renderer
{
public:
	Renderer();
	void Render();
	void Init();
	void Update(const float& time, const float& dt);
	void SetJuliaPos(const glm::vec2& p);
	void SetCameraPos(const glm::vec3& p);
	glm::vec3 GetCameraPos();
	glm::mat4 GetCameraMat();
	void SetCameraMat(glm::mat4 m);
private:
	Quad rootVolume;
	std::vector<SceneObject*> m_sceneObjects;
	ShaderProgram m_shader = ShaderProgram(ShaderPath("vertex.glsl"), ShaderPath("fragment.glsl"));
	VAOLoader m_loader;
	glm::vec2 m_juliaPos;
	glm::vec3 m_cameraPos;
	glm::mat4 m_camMat;
};

