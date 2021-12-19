#include "ShaderProgram.h"
#include "Shader.h";
ShaderProgram& ShaderProgram::Use()
{
	glUseProgram(m_programHandle);
	glDeleteShader(m_vertexShader.GetShaderHandle());
	glDeleteShader(m_fragmentShader.GetShaderHandle());
	return (*this);
}

void ShaderProgram::attach()
{
	glAttachShader(m_programHandle, m_vertexShader.GetShaderHandle());
	glAttachShader(m_programHandle, m_fragmentShader.GetShaderHandle());
}
void ShaderProgram::link()
{
	glLinkProgram(m_programHandle);
}
