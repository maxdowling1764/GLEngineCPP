#include "Renderer.h"
#include "Model.h"
Renderer::Renderer() :
	rootVolume(Quad(glm::vec3(-1.0f, 1.0f, 0.5f), 
					glm::vec3(1.0f, 1.0f, 0.5f), 
					glm::vec3(1.0f, -1.0f, 0.5f), 
					glm::vec3(-1.0f, -1.0f, 0.5f))), 
	m_sceneObjects({}),
	m_mainCamera(Camera(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.1f, 100.0f, 1.0f, 45.0f))
{
	m_model = Model();
	m_model.LoadModel("models/suzanne.obj");
	m_loader = VAOLoader();
}

void Renderer::Render()
{
	// TODO Bind to Framebuffer
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	m_basicShader.Use();
	m_model.Render(m_basicShader);
}

void Renderer::Update(const float& time, const float& dt)
{
	m_mainCamera.m_position = 5.0f*glm::vec3(glm::cos(time), 0.0f, -glm::sin(time));
	m_mainCamera.m_orientation.a = glm::vec3(-glm::cos(time), 0.0f, glm::sin(time));
	m_mainCamera.Update(dt);
	m_juliaShader.u_SetFloat("time", time);
	m_basicShader.u_SetMat4("view", m_mainCamera.GetViewMatrix());
	m_model.transform = glm::rotate(m_model.transform, glm::radians(0.5f), glm::vec3(1.0f, 1.0f, 0.0f));
}

void Renderer::Init()
{
	//// Create Framebuffer
	//glGenFramebuffers(1, &m_framebuffer);
	//glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

	//// Create Texture
	//glGenTextures(1, &m_texturebuffer);
	//glBindTexture(GL_TEXTURE_2D, m_texturebuffer);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 800, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//

	//// Bind Framebuffer to Texture
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texturebuffer, 0);

	//// Create render buffer
	//glGenRenderbuffers(1, &m_renderbuffer);
	//glBindRenderbuffer(GL_RENDERBUFFER, m_renderbuffer);
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 800);
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_renderbuffer);
	//
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//m_juliaShader.Init();
	//m_postProcShader.Init();
	m_basicShader.Init();
	m_basicShader.Use();
	m_model.Init(m_loader);
	m_mainCamera.m_position = glm::vec3(0.0f, 0.0f, 0.0f);
	m_basicShader.u_SetMat4("projection", m_mainCamera.GetProjectionMatrix());
	m_basicShader.u_SetMat4("view", m_mainCamera.GetViewMatrix());
	//m_model.transform = glm::translate(m_model.transform, glm::vec3(0.0f, 0.0f, 2.0f));
	
	
	//m_juliaShader.u_SetFloat("time", 1.0f);
	glEnable(GL_BLEND);
	
	rootVolume.Init(m_loader);
	
}

Renderer::~Renderer()
{
	for (int i = 0; i < m_sceneObjects.size(); i++)
	{
		delete m_sceneObjects[i];
	}
	m_sceneObjects.clear();
}
