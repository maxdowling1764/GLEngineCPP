#include "Renderer.h"
#include "Model.h"
#include <stdint.h>
#include "Util.h"
#include "VolumetricShaderProgram.h"
Renderer::Renderer() : rootVolume(Quad(glm::vec3(-1.0f, 1.0f, 0.5f), 
										glm::vec3(1.0f, 1.0f, 0.5f), 
										glm::vec3(1.0f, -1.0f, 0.5f), 
										glm::vec3(-1.0f, -1.0f, 0.5f))), 
						m_sceneObjects({}),
						texture(Texture3D(256, 256, 109)),
						m_shader(VolumetricShaderProgram())
{
	m_loader = VAOLoader();
	m_juliaPos = glm::vec2(1.0f, 0.0f);
	m_activeCamera = Camera();
}

void Renderer::Render()
{
	rootVolume.Render(m_shader);
}

void Renderer::SetJuliaPos(const glm::vec2& p)
{
	m_juliaPos = glm::vec2(p);
	m_shader.u_SetVec2("julia_pos", p);
}

void Renderer::SetActiveCamera(Camera& cam)
{
	m_activeCamera = cam;
}

Camera* Renderer::GetActiveCamera()
{
	return &m_activeCamera;
}

void Renderer::Update(const float& time, const float& dt)
{
	m_shader.u_SetFloat("time", time);
	m_shader.u_SetVec2("julia_pos", m_juliaPos); 
	m_shader.u_SetVec3("cam_forward", m_activeCamera.GetForward());
	m_shader.u_SetVec3("cam_up", m_activeCamera.GetUp());
	m_shader.u_SetVec3("cam_pos", m_activeCamera.GetPosition());
}

void Renderer::Init()
{
	m_shader.Init();
	m_shader.Use();
	texture.RenderInit();
	std::string path = "resources/textures/volume/MRbrain/data.dat";
	std::vector<int16_t> volumedata = read_file_raw<int16_t>(path);
	std::vector<float> floatdata = std::vector<float>();

	std::int16_t max_vol_data = 1;
	if (volumedata.size() > 0)
	{
		std::cout << "Read " << volumedata.size() * sizeof(int16_t) << " bytes from " << path << std::endl;
		for (int i = 0; i < volumedata.size(); i++)
		{
			volumedata[i] =  (0x00FF & (volumedata[i] >> 8)) | (0xFF00 & (volumedata[i] << 8));
			if (volumedata[i] > max_vol_data)
			{
				max_vol_data = volumedata[i];
			}
		}

		for (int i = 0; i < volumedata.size(); i++)
		{
			float v = (float)volumedata[i] / max_vol_data;
			if (v > 0.3f && v < 0.4)
			{
				floatdata.push_back(1.0f);
				floatdata.push_back(0.0f);
				floatdata.push_back(1.0f);
				floatdata.push_back(1.0f);
			}
			else if (v > 0.4 && v < 0.5)
			{
				floatdata.push_back(0.0f);
				floatdata.push_back(0.0f);
				floatdata.push_back(1.0f);
				floatdata.push_back(1.0f);
			}
			else if (v > 0.5)
			{
				floatdata.push_back(0.0f);
				floatdata.push_back(1.0f);
				floatdata.push_back(1.0f);
				floatdata.push_back(1.0f);
			}
			else
			{
				floatdata.push_back(1.0f);
				floatdata.push_back(1.0f);
				floatdata.push_back(1.0f);
				floatdata.push_back(1.0f);
			}
		}
		std::cout << std::endl;
		std::cout << "Head of " << path << ": ";
		for (int i = 0; i < 100; i++)
		{
			std::cout << floatdata[i] << " ";
		}
		std::cout << std::endl;
		 
	}
	else
	{
		std::cerr << "Unable to read file: " << path << std::endl;
	}
		
	const int w = 256; const int h = 256; const int d = 109;
	if (volumedata.size() != w * h * d)
		std::cerr << "Expected " << w * h * d * sizeof(int16_t)
				  << " bytes; Read " << volumedata.size() * sizeof(int16_t) << std::endl;
	texture.SetData(floatdata);
	texture.Load();
	texture.Bind(m_shader, "tex", GL_TEXTURE0);
	

	m_shader.u_SetVec2("julia_pos", m_juliaPos);
	m_shader.u_SetVec3("cam_pos", m_activeCamera.GetPosition());
	m_shader.u_SetVec3("cam_forward", m_activeCamera.GetForward());
	m_shader.u_SetVec3("cam_up", m_activeCamera.GetUp());
	m_shader.u_SetMat4("projection", glm::ortho(-1.0f, 1.0f, 1.0f, -1.0f, 0.0f, -100.0f));
	glEnable(GL_BLEND);
	
	rootVolume.Init(m_loader);
	
}

void Renderer::Cleanup()
{
	texture.Free();
}