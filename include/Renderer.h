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
#include "FragPositionShader.h"

struct FrameBuffer {
	unsigned int framebuffer;
	Texture2D texture;
	Texture2D depthbuffer;
	unsigned int renderbuffer;
	size_t width, height;
	FrameBuffer(size_t width, size_t height) : width(width), height(height), texture(Texture2D(width, height, "framebuffer")) {};
};

struct DomainBuffer {
	unsigned int front_framebuffer, back_framebuffer;
	size_t width, height;
	Texture2D front_pos_buffer, back_pos_buffer;
	Texture2D front_depthbuffer, back_depthbuffer;
	DomainBuffer(size_t width, size_t height) : width(width), height(height), front_depthbuffer(Texture2D(width, height)), back_depthbuffer(Texture2D(width, height)) {};
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
	Quad m_screenPlane;
	Quad m_groundPlane;
	std::vector<SceneObject> m_sceneObjects;
	Texture3D volume;
	Texture2D texture;
	Texture2D m_screenTexture;
	ShaderProgram* m_shader;
	OverlayShaderProgram m_screenShader;
	FragPositionShader m_modelPositionShader;
	VAOLoader m_loader;
	Camera m_activeCamera;
	Model m_model;
	Model m_model2;
	Mesh m_mesh;
	FrameBuffer m_framebuffer;
	DomainBuffer m_domainBuffer;
	bool mesh_is_loaded = false;	
};

