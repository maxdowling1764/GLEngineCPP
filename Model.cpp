#include "Model.h"

void Model::Init(VAOLoader& loader)
{
	loader.LoadMesh(m_mesh);
}

void Model::Render(ShaderProgram& shader)
{
	m_mesh.Render(shader);
}
