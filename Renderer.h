#pragma once
#include <vector>
#include "VolumeElement.h"
#include "ShaderProgram.h"
#include "SceneObject.h"
#include "glm.hpp"
#include "Camera.h"
class Renderer
{
public:
	Renderer();
	void Render();
	void Init();
	void Update(const float& time, const float& dt);
	void SetJuliaPos(const glm::vec2& p);
	Camera* GetActiveCamera();
	void SetActiveCamera(Camera& cam);
private:
	Quad rootVolume;
	std::vector<SceneObject*> m_sceneObjects;
	ShaderProgram m_shader = ShaderProgram(ShaderPath("vertex.glsl"), ShaderPath("fragment.glsl"));
	VAOLoader m_loader;
	glm::vec2 m_juliaPos;
	Camera m_activeCamera;
};

