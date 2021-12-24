#include "Shader.h"
#include <glad/glad.h>
#include <iostream>
Shader::Shader(std::string shaderSource, unsigned int type)
{
	m_src = shaderSource;
	m_type = type;
	m_path = "";

}
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
Shader::Shader(ShaderPath path, unsigned int type)
{
	m_path = path.path;
	m_type = type;
	m_src = read_file(m_path);
	std::cout << m_path << std::endl;
	std::cout << m_src << std::endl;
}
