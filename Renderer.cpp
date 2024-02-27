#include "Renderer.h"
#include "Model.h"
#include <stdint.h>
#include "Util.h"
#include "VolumetricShaderProgram.h"
#include "PolygonShaderProgram.h"
#include "ModelParser.h"

#define TEX_WIDTH 256
#define TEX_HEIGHT 256
#define TEX_DEPTH 109

Renderer::Renderer() : 
	rootVolume(Quad(glm::vec3(-1.0f, 1.0f, 0.5f),
					glm::vec3(1.0f, 1.0f, 0.5f),
					glm::vec3(-1.0f, -1.0f, 0.5f),
					glm::vec3(1.0f, -1.0f, 0.5f))),
	m_sceneObjects({}),
	volume(Texture3D(TEX_WIDTH, TEX_HEIGHT, TEX_DEPTH)),
	texture(Texture2D(TEX_WIDTH, TEX_HEIGHT)),
	m_loader(VAOLoader()),
	m_activeCamera(Camera()),
	m_shader(new PolygonShaderProgram(m_activeCamera)),
	m_mesh(Mesh())
{
	std::string meshpath = "resources/models/smooth_suzzanne.obj";
	std::vector<glm::vec4> vecdata = load_volume_data("resources/textures/volume/MRbrain/data.dat");
	volume.SetData(vecdata);
	texture.SetData(vecdata, TEX_WIDTH*TEX_HEIGHT*50, TEX_WIDTH*TEX_HEIGHT*51);
	mesh_is_loaded = ModelParser::parse_obj(meshpath, m_mesh);
	m_model = Model(m_mesh);
}

void Renderer::Render()
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_model.Render(*m_shader);
	rootVolume.Render(*m_shader);
}

void Renderer::Init()
{
	m_shader->RenderInit();
	m_shader->Use();
	m_shader->InitUniforms();

	texture.RenderInit();
	volume.RenderInit();

	//volume.Bind(m_shader, "tex", GL_TEXTURE0);
	texture.Bind(m_shader, "diffuse", 0);
	rootVolume.Init(m_loader);
	if (mesh_is_loaded)
	{
		m_model.Init(m_loader);
	}
	
	glEnable(GL_BLEND);
}

void Renderer::Cleanup()
{
	volume.Free();
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