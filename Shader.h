#pragma once
#include <string>

class Shader
{
private:
	unsigned int m_shaderHandle;

public:
	Shader(std::string shaderSource, unsigned int type);
	const unsigned int GetShaderHandle();
	
};