#include "Renderer.h"
#include "Model.h"
#include <stdint.h>
#include "Util.h"
#include "VolumetricShaderProgram.h"
#include "PolygonShaderProgram.h"
#include "OverlayShaderProgram.h"
#include "ModelParser.h"
#include "CLMesh.h"
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
				vecdata.push_back(glm::vec4(0, 0, 0, 1));
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
	m_screenPlane(Quad(glm::vec3(-1.0f, 1.0f, 0.5f),
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
	m_screenShader(OverlayShaderProgram(m_activeCamera)),
	m_mesh(Mesh()),
	m_groundPlane(Quad(glm::vec3(-1.0f, 0, -1.0f),
		glm::vec3(1.0f, 0, -1.0f),
		glm::vec3(-1.0f, 0, 1.0f),
		glm::vec3(1.0f, 0, 1.0f))),
	m_domainBuffer(DomainBuffer(1920, 1080)),
	m_modelPositionShader(FragPositionShader(m_activeCamera)),
	m_framebuffer(FrameBuffer(1920, 1080))
{
	std::string meshpath = "resources/models/cube_normals.obj";
	std::vector<glm::vec4> vecdata = load_volume_data("resources/textures/volume/MRbrain/data.dat");
	volume.SetData(vecdata);
	texture.SetData(vecdata, TEX_WIDTH*TEX_HEIGHT*50, TEX_WIDTH*TEX_HEIGHT*51);
	mesh_is_loaded = ModelParser::parse_obj(meshpath, m_mesh);
	m_model = Model(m_mesh);
	m_model2 = Model(m_mesh);

	std::vector<cl::Platform> platforms;
	cl::Platform::get(&platforms);
	auto platform = platforms.front();
	std::vector<cl::Device> devices;
	platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);

	cl::Device* selectedDevice = nullptr;
	if (!devices.empty())
	{
		selectedDevice = &devices[0];
		for (auto dev : devices)
		{
			if (dev.getInfo<CL_DEVICE_TYPE>() == CL_DEVICE_TYPE_GPU)
			{
				selectedDevice = &dev;
			}
		}
	}

	if (selectedDevice)
	{
		cl::Context context(*selectedDevice);
		CLMesh clm = CLMesh(m_mesh, selectedDevice, &context);

		clm.EnqueueMeshOp(m_mesh, CLMesh::CLMeshOp::BIN, *selectedDevice, context);
	}
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
	glCullFace(GL_BACK);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_shader->Use();
	//texture.Bind(m_shader, "diffuse", 0);
	m_shader->Update(0.0f, 0.0f);
	//m_model.Render(*m_shader);
	m_groundPlane.Render(*m_shader);

	glBindFramebuffer(GL_FRAMEBUFFER, m_domainBuffer.front_framebuffer);
	m_modelPositionShader.Use();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	m_model.Render(m_modelPositionShader);
	
	glBindFramebuffer(GL_FRAMEBUFFER, m_domainBuffer.back_framebuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glCullFace(GL_FRONT);
	m_model.Render(m_modelPositionShader);


	// Post-Processing
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glClear(GL_COLOR_BUFFER_BIT);
	/* Now render the viewport quad directly to the screen */
	m_screenShader.Use();
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_framebuffer.texture.Handle());
	m_screenShader.u_Set1i("colorbuffer", 1);
	
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_framebuffer.depthbuffer.Handle());
	m_screenShader.u_Set1i("depthbuffer", 2);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, m_domainBuffer.front_depthbuffer.Handle());
	m_screenShader.u_Set1i("domaindepth_front", 3);
	
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, m_domainBuffer.back_depthbuffer.Handle());
	m_screenShader.u_Set1i("domaindepth_back", 4);

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, m_domainBuffer.front_pos_buffer.Handle());
	m_screenShader.u_Set1i("domainpos_front", 5);

	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, m_domainBuffer.back_pos_buffer.Handle());
	m_screenShader.u_Set1i("domainpos_back", 6);

	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_3D, volume.Handle());
	m_screenShader.u_Set1i("volumeData", 7);

	m_screenPlane.Render(m_screenShader);
}

void init_domainbuffer(DomainBuffer* res)
{
	glGenFramebuffers(1, &(res->front_framebuffer));
	glGenFramebuffers(1, &(res->back_framebuffer));
	
	// Init front face buffer
	glBindFramebuffer(GL_FRAMEBUFFER, res->back_framebuffer);
	res->back_depthbuffer.RenderInit();
	res->back_pos_buffer.RenderInit();
	glBindTexture(GL_TEXTURE_2D, res->back_depthbuffer.Handle());
	glTexParameteri(res->back_depthbuffer.Handle(), GL_DEPTH_TEXTURE_MODE, GL_DEPTH_COMPONENT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, res->width, res->height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, res->back_depthbuffer.Handle(), 0);

	glBindTexture(GL_TEXTURE_2D, res->back_pos_buffer.Handle());
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, res->width, res->height, 0, GL_RGBA, GL_FLOAT, nullptr);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, res->back_pos_buffer.Handle(), 0);

	// Init back face buffer
	glBindFramebuffer(GL_FRAMEBUFFER, res->front_framebuffer);
	res->front_depthbuffer.RenderInit();
	res->front_pos_buffer.RenderInit();
	glBindTexture(GL_TEXTURE_2D, res->front_depthbuffer.Handle());
	glTexParameteri(res->front_depthbuffer.Handle(), GL_DEPTH_TEXTURE_MODE, GL_DEPTH_COMPONENT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, res->width, res->height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, res->front_depthbuffer.Handle(), 0);
	
	glBindTexture(GL_TEXTURE_2D, res->front_pos_buffer.Handle());
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, res->width, res->height, 0, GL_RGBA, GL_FLOAT, nullptr);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, res->front_pos_buffer.Handle(), 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void init_framebuffer(FrameBuffer* res)
{
	glGenFramebuffers(1, &(res->framebuffer));
	glBindFramebuffer(GL_FRAMEBUFFER, res->framebuffer);

	res->texture.RenderInit();

	//glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, res->texture.Handle());
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, res->width, res->height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, res->texture.Handle(), 0);


	res->depthbuffer.RenderInit();

	//glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, res->depthbuffer.Handle());
	glTexParameteri(res->depthbuffer.Handle(), GL_DEPTH_TEXTURE_MODE, GL_DEPTH_COMPONENT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, res->width, res->height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, res->depthbuffer.Handle(), 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void Renderer::Init()
{
	std::cout << "GL Version: " << glGetString(GL_VERSION) << std::endl;
	init_framebuffer(&m_framebuffer);
	init_domainbuffer(&m_domainBuffer);
	m_shader->RenderInit();
	m_shader->Use();
	m_shader->InitUniforms();

	m_modelPositionShader.RenderInit();
	m_modelPositionShader.Use();
	m_modelPositionShader.InitUniforms();
	glm::mat4 m = glm::translate(glm::identity<glm::mat4>(), glm::vec3(0.0f, 3.0f, 0.0f));
	m_model.SetTransform(m);
	texture.RenderInit();
	m_screenTexture.RenderInit();
	volume.RenderInit();

	/* Initialize the framebuffer to be bound to a texture uniform in the overlay shader */

	m_screenShader.RenderInit();
	m_screenShader.Use();
	m_screenShader.InitUniforms();
	m_screenShader.u_Set1i("colorbuffer", 1);
	m_screenShader.u_Set1i("depthbuffer", 2);
	m_screenShader.u_Set1i("domainpos_front", 5);

	m_screenShader.u_SetFloat("t", 0.1f);

	//volume.Bind(m_shader, "tex", GL_TEXTURE0);
	
	m_screenPlane.Init(m_loader);
	if (mesh_is_loaded)
	{
		m_model.Init(m_loader);
		m_model2.Init(m_loader);
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

	glm::mat4 rot = glm::rotate(
						glm::rotate(glm::translate(glm::identity<glm::mat4>(), glm::vec3(0.0f, glm::sin(time), 0.0f)), glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
						glm::radians(time * 100.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	m_model.SetTransform(rot);
	m_modelPositionShader.Use();
	m_modelPositionShader.Update(time, dt);
	
	
	//glm::vec3 campos = m_activeCamera.GetPosition();
	//
	//glm::vec3 modelpos = glm::vec3(m_model.GetTransform()[3]);
	//glm::vec3 camdir = glm::normalize(campos-modelpos);
	//glm::mat4 tmp = glm::inverse(glm::lookAt(modelpos + camdir*dt, 10.0f*camdir, glm::vec3(0,1,0)));
	//m_model.SetTransform(tmp);
	//m_shader->u_SetMat4("model", tmp);
}