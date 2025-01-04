#pragma once
#include "FragmentShader.h"
#include "VertexShader.h"
#include <gtc/type_ptr.hpp>
#include <iostream>
class ShaderProgram
{
public:
	//ShaderProgram(ResourceLoader& loader, std::string& vertexPath, std::string& fragmentPath);
	ShaderProgram(std::string vertex_source, std::string fragment_source): 
		m_vertexShader(VertexShader(vertex_source)), 
		m_fragmentShader(FragmentShader(fragment_source))
	{
		std::cout << m_fragmentShader.GetSource();
		std::cout << m_vertexShader.GetSource();
	};

	virtual ~ShaderProgram() {};

	ShaderProgram(ShaderPath vertexShader, ShaderPath fragmentShader);
	ShaderProgram(VertexShader vs, FragmentShader fs);
	
	ShaderProgram& Use();
	void RenderInit();

	/* Uniform Mutators */
	void u_SetMat4(const char* name, const glm::mat4& val);
	void u_SetVec2(const char* name, const glm::vec2& val);
	void u_SetVec3(const char* name, const glm::vec3& val);
	void u_SetFloat(const char* name, const float& val);
	void u_Set1i(const char* name, const int& val);

	
	virtual void InitUniforms() = 0;
	virtual void Update(const float& time, const float& dt) = 0;

private:
	VertexShader m_vertexShader;
	FragmentShader m_fragmentShader;

	unsigned int m_programHandle = 0;
	void attach();
	void link();
};