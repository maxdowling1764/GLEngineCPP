#include "Renderer.h"
#include "Model.h"
Renderer::Renderer() : rootVolume(Quad(glm::vec3(-1.0f, 1.0f, 0.5f), 
										glm::vec3(1.0f, 1.0f, 0.5f), 
										glm::vec3(1.0f, -1.0f, 0.5f), 
										glm::vec3(-1.0f, -1.0f, 0.5f))), 
						m_sceneObjects({}),
						texture(Texture3D(255, 255, 255))
{
	FragmentShader fs = FragmentShader(ShaderPath("shaders/fragment.glsl"));
	//std::cout << fs.GetSource() << std::endl;
	VertexShader vs = VertexShader(ShaderPath("shaders/vertex.glsl"));
	//std::cout << vs.GetSource() << std::endl;
	m_shader = ShaderProgram(vs, fs);
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
	const int w = 255; const int h = 255; const int d = 255;

	glm::vec4 val = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
	for (int i = 0; i < w; i++)
	{
		for (int j = 0; j < h; j++)
		{
			for (int k = 0; k < d; k++)
			{
				float s = 1.0f / 255.0f;
				glm::vec4 v = s * glm::vec4((float)i, (float)j, (float)k, 1.0f);
				texture.SetValue(i, j, k, v);
			}
		}
	}
	
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