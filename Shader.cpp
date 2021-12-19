#include "Shader.h"
#include <glad/glad.h>

Shader::Shader(std::string shaderSource, unsigned int type)
{
	const char* src = shaderSource.c_str();
	m_shaderHandle = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(m_shaderHandle, 1, &src, NULL);
	glCompileShader(m_shaderHandle);

}
const unsigned int Shader::GetShaderHandle()
{
	return m_shaderHandle;
}
