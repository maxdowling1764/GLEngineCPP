#include "Renderer.h"
#include "Model.h"
#include <stdint.h>
#include "Util.h"
#include "VolumetricShaderProgram.h"
#include "PolygonShaderProgram.h"
#include "OverlayShaderProgram.h"
#include "ModelParser.h"

#define TEX_WIDTH 256
#define TEX_HEIGHT 256
#define TEX_DEPTH 109
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
Renderer::Renderer() :
	rootVolume(Quad(glm::vec3(-1.0f, 1.0f, 0.5f),
		glm::vec3(1.0f, 1.0f, 0.5f),
		glm::vec3(-1.0f, -1.0f, 0.5f),
		glm::vec3(1.0f, -1.0f, 0.5f))),
	m_sceneObjects({}),
	volume(Texture3D(TEX_WIDTH, TEX_HEIGHT, TEX_DEPTH)),
	texture(Texture2D(TEX_WIDTH, TEX_HEIGHT)),
	m_screenTexture(Texture2D(1920, 1080)),
	m_loader(VAOLoader()),
	m_activeCamera(Camera()),
	m_shader(new PolygonShaderProgram(m_activeCamera)),
	m_overlayShader(OverlayShaderProgram(m_activeCamera)),
	m_mesh(Mesh()),
	m_groundPlane(Quad(glm::vec3(-1.0f, 0, -1.0f),
		glm::vec3(1.0f, 0, -1.0f),
		glm::vec3(-1.0f, 0, 1.0f),
		glm::vec3(1.0f, 0, 1.0f)))
{
	std::string meshpath = "resources/models/smooth_suzzanne.obj";
	std::vector<glm::vec4> vecdata = load_volume_data("resources/textures/volume/MRbrain/data.dat");
	volume.SetData(vecdata);
	texture.SetData(vecdata, TEX_WIDTH*TEX_HEIGHT*50, TEX_WIDTH*TEX_HEIGHT*51);
	mesh_is_loaded = ModelParser::parse_obj(meshpath, m_mesh);
	m_model = Model(m_mesh);
}

void read_framebuffer()
{
	
}

void draw_framebuffer(FrameBuffer* fb)
{
	glBindFramebuffer(GL_FRAMEBUFFER, fb->framebuffer);
}


// void Renderer::SetRenderTarget(??);

void Renderer::Render()
{

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	/* Render scene to framebuffer */
	
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer.framebuffer);
	glEnable(GL_DEPTH_TEST);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_shader->Use();
	texture.Bind(m_shader, "diffuse", 0);
	m_shader->Update(0.0f, 0.0f);
	m_model.Render(*m_shader);
	m_groundPlane.Render(*m_shader);


	// Post-Processing
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT);
	/* Now render the viewport quad directly to the screen */
	m_overlayShader.Use();
	m_framebuffer.texture.Bind(&m_overlayShader, "framebuffer", 0);
	rootVolume.Render(m_overlayShader);
}

void init_framebuffer(FrameBuffer* res)
{
	glGenFramebuffers(1, &(res->framebuffer));
	glBindFramebuffer(GL_FRAMEBUFFER, res->framebuffer);

	res->texture.RenderInit();

	glBindTexture(GL_TEXTURE_2D, res->texture.Handle());
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1920, 1080, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, res->texture.Handle(), 0);

	glGenRenderbuffers(1, &(res->renderbuffer));
	glBindRenderbuffer(GL_RENDERBUFFER, res->renderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1920, 1080);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, res->renderbuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void Renderer::Init()
{
	init_framebuffer(&m_framebuffer);
	m_shader->RenderInit();
	m_shader->Use();
	m_shader->InitUniforms();

	texture.RenderInit();
	m_screenTexture.RenderInit();
	volume.RenderInit();

	/* Initialize the framebuffer to be bound to a texture uniform in the overlay shader */

	m_overlayShader.RenderInit();
	m_overlayShader.Use();
	m_overlayShader.InitUniforms();

	//volume.Bind(m_shader, "tex", GL_TEXTURE0);
	
	rootVolume.Init(m_loader);
	if (mesh_is_loaded)
	{
		m_model.Init(m_loader);
	}
	glm::mat4 tmp = glm::scale(glm::identity<glm::mat4>(), glm::vec3(20.0f));
	m_groundPlane.SetTransform(tmp);
	m_groundPlane.Init(m_loader);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void Renderer::Cleanup()
{
	volume.Free();
	texture.Free();
	delete(m_shader);
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
	m_shader->Use();
	m_shader->Update(time, dt);
	
	//glm::vec3 campos = m_activeCamera.GetPosition();
	//
	//glm::vec3 modelpos = glm::vec3(m_model.GetTransform()[3]);
	//glm::vec3 camdir = glm::normalize(campos-modelpos);
	//glm::mat4 tmp = glm::inverse(glm::lookAt(modelpos + camdir*dt, 10.0f*camdir, glm::vec3(0,1,0)));
	//m_model.SetTransform(tmp);
	//m_shader->u_SetMat4("model", tmp);
}