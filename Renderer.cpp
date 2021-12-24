#include "Renderer.h"
#include "Model.h"

Renderer::Renderer() :rootVolume(Quad(glm::vec3(-1.0f, 1.0f, 0.5f), glm::vec3(1.0f, 1.0f, 0.5f), glm::vec3(1.0f, -1.0f, 0.5f), glm::vec3(-1.0f, -1.0f, 0.5f))), m_sceneObjects({})
{
	FragmentShader fs = FragmentShader(ShaderPath("shaders/fragment.glsl"));
	//std::cout << fs.GetSource() << std::endl;
	VertexShader vs = VertexShader(ShaderPath("shaders/vertex.glsl"));
	//std::cout << vs.GetSource() << std::endl;
	m_shader = ShaderProgram(vs, fs);
	m_loader = VAOLoader();
}

void Renderer::Render()
{
	rootVolume.Render(m_shader);
}

void Renderer::Update(const float& time, const float& dt)
{
	m_shader.u_SetFloat("time", time);
}

void Renderer::Init()
{
	m_shader.Init();
	m_shader.Use();
	m_shader.u_SetMat4("projection", glm::ortho(-1.0f, 1.0f, 1.0f, -1.0f, 0.0f, -100.0f));
	m_shader.u_SetFloat("time", 1.0f);
	glEnable(GL_BLEND);
	
	rootVolume.Init(m_loader);
	
}