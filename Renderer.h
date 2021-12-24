#pragma once
#include <vector>
#include "VolumeElement.h"
#include "ShaderProgram.h"
#include "SceneObject.h"
class Renderer
{
public:
	Renderer();
	void Render();
	void Init();
	void Update(const float& time, const float& dt);
private:
	Quad rootVolume;
	std::vector<SceneObject*> m_sceneObjects;
	ShaderProgram m_shader = ShaderProgram(ShaderPath("vertex.glsl"), ShaderPath("fragment.glsl"));
	VAOLoader m_loader;
};

