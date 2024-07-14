#include "Model.h"

void Model::Init(VAOLoader& loader)
{
	if (!loader.LoadMesh(m_mesh))
	{
		std::cerr << "Failed to load mesh to VAO\n";
	}
	else
	{
		std::cout << "Loaded mesh to VAOId: " << m_mesh.vao << " | VBOId: " << m_mesh.vbo << std::endl;
	}
}

void Model::Render(ShaderProgram& shader)
{
	shader.u_SetMat4("model", m_transform);
	m_mesh.Render(shader);
	shader.u_SetMat4("model", glm::identity<glm::mat4>());
}
