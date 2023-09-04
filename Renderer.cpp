#include "Renderer.h"
#include "Model.h"

Renderer::Renderer() : rootVolume(Quad(glm::vec3(-1.0f, 1.0f, 0.5f), 
										glm::vec3(1.0f, 1.0f, 0.5f), 
										glm::vec3(1.0f, -1.0f, 0.5f), 
										glm::vec3(-1.0f, -1.0f, 0.5f))), 
						m_sceneObjects({})
{
	FragmentShader fs = FragmentShader(ShaderPath("shaders/fragment.glsl"));
	//std::cout << fs.GetSource() << std::endl;
	VertexShader vs = VertexShader(ShaderPath("shaders/vertex.glsl"));
	//std::cout << vs.GetSource() << std::endl;
	m_shader = ShaderProgram(vs, fs);
	m_loader = VAOLoader();
	m_cameraPos = glm::vec3(1.0f, 1.0f, -4.0f);
	m_juliaPos = glm::vec2(1.0f, 0.0f);
	m_camMat = glm::mat4(
		glm::vec4(glm::vec3(1.0f, 0.0f, 0.0f), 1.0f),
		glm::vec4(glm::vec3(0.0f, 1.0f, 0.0f), 1.0f),
		glm::vec4(glm::vec3(0.0f, 0.0f, 1.0f), 1.0f),
		glm::vec4(m_cameraPos, 1.0f));

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

void Renderer::SetCameraPos(const glm::vec3& p)
{
	m_cameraPos = glm::vec3(p);
	m_camMat = glm::mat4(
		glm::vec4(glm::vec3(1.0f, 0.0f, 0.0f), 1.0f),
		glm::vec4(glm::vec3(0.0f, 1.0f, 0.0f), 1.0f),
		glm::vec4(glm::vec3(0.0f, 0.0f, 1.0f), 1.0f),
		glm::vec4(m_cameraPos, 1.0f));
}

glm::vec3 Renderer::GetCameraPos()
{
	return m_cameraPos;
}

glm::mat4 Renderer::GetCameraMat()
{
	return m_camMat;
}

void Renderer::SetCameraMat(glm::mat4 m)
{
	m_camMat = m;
}

void Renderer::Update(const float& time, const float& dt)
{
	m_shader.u_SetFloat("time", time);
	m_shader.u_SetVec2("julia_pos", m_juliaPos);
	m_shader.u_SetMat4("cam_mat", m_camMat);
}

void Renderer::Init()
{
	m_shader.Init();
	m_shader.Use();
	m_shader.u_SetVec2("julia_pos", m_juliaPos);
	m_shader.u_SetMat4("cam_mat", m_camMat);
	m_shader.u_SetMat4("projection", glm::ortho(-1.0f, 1.0f, 1.0f, -1.0f, 0.0f, -100.0f));
	glEnable(GL_BLEND);
	
	rootVolume.Init(m_loader);
	
}