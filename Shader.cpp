#include "Shader.h"
#include <glad/glad.h>
#include <iostream>

const unsigned int Shader::GetShaderHandle()
{
	return m_shaderHandle;
}

void Shader::Init()
{
	m_shaderHandle = glCreateShader(m_type);
	const char* csrc = m_src.c_str();
	glShaderSource(m_shaderHandle, 1, &csrc, NULL);
	glCompileShader(m_shaderHandle);
}
std::string Shader::GetSource()
{
	return m_src;
}
