#pragma once
#include <vector>
#include "Quad.h"
#include "ShaderProgram.h"
#include "OverlayShaderProgram.h"
#include "SceneObject.h"
#include "glm.hpp"
#include "Camera.h"
#include "Texture3D.h"
#include "Texture2D.h"

struct FrameBuffer {
	unsigned int framebuffer;
	Texture2D texture;
	unsigned int renderbuffer;
	FrameBuffer() : texture(Texture2D(1920, 1080, "framebuffer")) {};
};



class Renderer
{
public:
	Renderer();
	~Renderer() 
	{
		if (m_shader != nullptr)
		{
			delete m_shader;
			m_shader = nullptr;
		}
	};
	void Render();
	void Init();
	void Update(const float& time, const float& dt);
	Camera* GetActiveCamera();
	void SetActiveCamera(Camera& cam);
	void Cleanup();
private:
	Quad rootVolume;
	Quad m_groundPlane;
	std::vector<SceneObject> m_sceneObjects;
	Texture3D volume;
	Texture2D texture;
	Texture2D m_screenTexture;
	ShaderProgram* m_shader;
	OverlayShaderProgram m_overlayShader;
	VAOLoader m_loader;
	Camera m_activeCamera;
	Model m_model;
	Mesh m_mesh;
	FrameBuffer m_framebuffer;
	bool mesh_is_loaded = false;

	
	
};

