#include "Model.h"

void Model::Init(VAOLoader& loader)
{
	if (!loader.LoadMesh(m_mesh))
	{
		std::cerr << "Failed to load mesh to VAO\n";
	}
	else
	{
		std::cout << "Loaded mesh to VAOId: " << m_mesh.GetVAOId() << " | VBOId: " << m_mesh.m_vbo << std::endl;
	}
}

void Model::Render(ShaderProgram& shader)
{
	m_mesh.Render(shader);
}
