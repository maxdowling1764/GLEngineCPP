#include "VolumeElement.h"
#include <glad/glad.h>
void Quad::Render(ShaderProgram& shader)
{
	m_model.Render(shader);
}

void Quad::Init(VAOLoader& loader)
{
	m_model.Init(loader);
}