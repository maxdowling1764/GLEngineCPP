#pragma once
#include <vector>
#include "Quad.h"
#include "ShaderProgram.h"
#include "SceneObject.h"
#include "glm.hpp"
#include "Camera.h"
#include "Texture3D.h"
#include "Texture2D.h"
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
	std::vector<SceneObject> m_sceneObjects;
	Texture3D volume;
	Texture2D texture;
	ShaderProgram* m_shader;
	VAOLoader m_loader;
	Camera m_activeCamera;
	Model m_model;
	Mesh m_mesh;
	bool mesh_is_loaded = false;

	std::vector <glm::vec4> load_volume_data(const std::string& volumepath)
	{
		std::vector<int16_t> volumedata = read_file_raw<int16_t>(volumepath);
		std::vector <glm::vec4> vecdata = std::vector<glm::vec4>();

		std::int16_t max_vol_data = 1;
		if (volumedata.size() > 0)
		{
			std::cout << "Read " << volumedata.size() * sizeof(int16_t)
				<< " bytes from " << volumepath << std::endl;

			// reverse endianness
			for (int i = 0; i < volumedata.size(); i++)
			{
				volumedata[i] = (0x00FF & (volumedata[i] >> 8)) | (0xFF00 & (volumedata[i] << 8));
				if (volumedata[i] > max_vol_data)
				{
					max_vol_data = volumedata[i];
				}
			}
			glm::mix(glm::vec4(1, 0, 1, 1), glm::vec4(1, 0, 1, 1), 1.0);
			for (int i = 0; i < volumedata.size(); i++)
			{
				float v = (float)volumedata[i] / max_vol_data;

				if (v > 0.3f && v < 0.4)
				{
					vecdata.push_back(glm::vec4(1, 0, 1, 1));
				}
				else if (v > 0.4 && v < 0.5)
				{
					vecdata.push_back(glm::vec4(0, 0, 1, 1));
				}
				else if (v > 0.5)
				{
					vecdata.push_back(glm::vec4(0, 1, 1, 1));
				}
				else
				{
					vecdata.push_back(glm::vec4(1, 1, 1, 1));
				}
			}
			std::cout << std::endl;
			std::cout << "Head of " << volumepath << ": ";
			for (int i = 0; i < 100; i++)
			{
				std::cout << volumedata[i] << " ";
			}
			std::cout << std::endl;

		}
		else
		{
			std::cerr << "Unable to read file: " << volumepath << std::endl;
		}

		const size_t w = 256; const size_t h = 256; const size_t d = 109;
		if (volumedata.size() != w * h * d)
			std::cerr << "Expected " << w * h * d * sizeof(int16_t)
			<< " bytes; Read " << volumedata.size() * sizeof(int16_t) << std::endl;
		return vecdata;
	}
	
};

