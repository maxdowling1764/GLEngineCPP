#include "Quad.h"
#include <glad/glad.h>
void Quad::Render(ShaderProgram& shader)
{
	m_model.Render(shader);
}
void Quad::Update(float dt)
{
	m_model.SetTransform(m_transform);
	m_model.Update();
}

void Quad::Init(VAOLoader& loader)
{
	m_model.Init(loader);
	m_model.SetTransform(m_transform);
}