#include "ShaderProgram.h"
#include "Shader.h";
#include <iostream>
ShaderProgram::ShaderProgram(ShaderPath vertexShader, ShaderPath fragmentShader):m_vertexShader(VertexShader(vertexShader)), m_fragmentShader(FragmentShader(fragmentShader))
{

}

ShaderProgram::ShaderProgram(VertexShader vs, FragmentShader fs):m_vertexShader(vs), m_fragmentShader(fs)
{

}

void ShaderProgram::Init()
{
	m_programHandle = glCreateProgram();
	m_vertexShader.Init();
	m_fragmentShader.Init();
	attach();
	link();
	int  success;
	char infoLog[512];
	glGetShaderiv(m_vertexShader.GetShaderHandle(), GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(m_vertexShader.GetShaderHandle(), 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	glGetShaderiv(m_fragmentShader.GetShaderHandle(), GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(m_fragmentShader.GetShaderHandle(), 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	std::cout << "SHader Compilation successful" << std::endl;
}
ShaderProgram& ShaderProgram::Use()
{
	glUseProgram(m_programHandle);
	return (*this);
}

void ShaderProgram::u_SetMat4(const char* name, const glm::mat4& val)
{
	glUniformMatrix4fv(glGetUniformLocation(m_programHandle, name), 1, false, glm::value_ptr(val));
}

void ShaderProgram::u_SetFloat(const char* name, const float& val)
{
	glUniform1f(glGetUniformLocation(m_programHandle, name), val);
}

void ShaderProgram::u_SetSampler2D(const char* name, const unsigned int& tex)
{
	glUniform1ui(glGetUniformLocation(m_programHandle, name), tex);
}

void ShaderProgram::attach()
{
	glAttachShader(m_programHandle, m_vertexShader.GetShaderHandle());
	glAttachShader(m_programHandle, m_fragmentShader.GetShaderHandle());
}
void ShaderProgram::link()
{
	int  success;
	char infoLog[512];
	glLinkProgram(m_programHandle);
	// check for linking errors
	glGetProgramiv(m_programHandle, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(m_programHandle, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
}
