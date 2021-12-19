#pragma once
#include "FragmentShader.h"
#include "VertexShader.h"
class ShaderProgram
{
public:
	//ShaderProgram(ResourceLoader& loader, std::string& vertexPath, std::string& fragmentPath);
	ShaderProgram(std::string& vertex_source, std::string& fragment_source): m_vertexShader(VertexShader(vertex_source)), m_fragmentShader(FragmentShader(fragment_source))
	{
		m_programHandle = glCreateProgram();
		attach();
		link();
		
	};
	ShaderProgram& Use();
	
private:
	VertexShader m_vertexShader;
	FragmentShader m_fragmentShader;

	unsigned int m_programHandle;
	void attach();
	void link();
};