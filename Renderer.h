#pragma once
#include <vector>
#include "Quad.h"
#include "ShaderProgram.h"
#include "SceneObject.h"
#include "glm.hpp"
#include "Camera.h"
#include "Texture3D.h"
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
	std::vector<SceneObject*> m_sceneObjects;
	Texture3D texture;
	ShaderProgram* m_shader;
	VAOLoader m_loader;
	Camera m_activeCamera;
};

