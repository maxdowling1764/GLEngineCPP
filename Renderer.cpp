#include "Renderer.h"
#include "Model.h"
#include <stdint.h>
#include "Util.h"
#include "VolumetricShaderProgram.h"
#include "PolygonShaderProgram.h"
#include "ModelParser.h"

Renderer::Renderer() : 
	rootVolume(Quad(glm::vec3(-1.0f, 1.0f, 0.5f),
					glm::vec3(1.0f, 1.0f, 0.5f),
					glm::vec3(-1.0f, -1.0f, 0.5f),
					glm::vec3(1.0f, -1.0f, 0.5f))),
	m_sceneObjects({}),
	texture(Texture3D(256, 256, 109)),
	m_loader(VAOLoader()),
	m_activeCamera(Camera()),
	m_shader(new PolygonShaderProgram(m_activeCamera)),
	m_mesh(Mesh())
{
	std::string meshpath = "resources/models/smooth_suzzanne.obj";
	
	mesh_is_loaded = ModelParser::parse_obj(meshpath, m_mesh);
	m_model = Model(m_mesh);
	
}

void Renderer::Render()
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_model.Render(*m_shader);
	//rootVolume.Render(*m_shader);
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
	m_shader->Update(time, dt);
}

void Renderer::Init()
{
	m_shader->RenderInit();
	m_shader->Use();
	texture.RenderInit();
	
	std::string path = "resources/textures/volume/MRbrain/data.dat";

	std::vector<int16_t> volumedata = read_file_raw<int16_t>(path);
	std::vector<float> floatdata = std::vector<float>();

	std::int16_t max_vol_data = 1;
	if (volumedata.size() > 0)
	{
		std::cout << "Read " << volumedata.size() * sizeof(int16_t) 
					<< " bytes from " << path << std::endl;
		for (int i = 0; i < volumedata.size(); i++)
		{
			// reverse endianness
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
			std::cout << volumedata[i] << " ";
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
	m_shader->InitUniforms();
	glEnable(GL_BLEND);
	
	if (mesh_is_loaded)
	{
		m_model.Init(m_loader);
	}
	
}

void Renderer::Cleanup()
{
	texture.Free();
}