#pragma once
#include <vector>
#include "VolumeElement.h"
#include "ShaderProgram.h"
#include "SceneObject.h"
#include "Camera.h"
class Renderer
{
public:
	Camera m_mainCamera;
	std::vector<SceneObject*> m_sceneObjects;

	Renderer();
	~Renderer();
	void Render();
	void Init();
	void Update(const float& time, const float& dt);
private:
	Quad rootVolume;
	
	Model m_model;
	unsigned int m_framebuffer, m_renderbuffer, m_texturebuffer;
	
	ShaderProgram m_juliaShader = ShaderProgram(ShaderPath("vertex.glsl"), ShaderPath("fragment.glsl"));
	ShaderProgram m_postProcShader = ShaderProgram(ShaderPath("v_postproc.glsl"), ShaderPath("f_postproc.glsl"));
	ShaderProgram m_basicShader = ShaderProgram(ShaderPath("v_basic.glsl"), ShaderPath("f_basic.glsl"));
	VAOLoader m_loader;
};

