#pragma once
#include <string>
#include "Util.h"

typedef struct ShaderPath 
{ 
	std::string path; 
	ShaderPath(std::string p) { path = p; }; 
} ShaderPath;
class Shader
{
	
private:
	unsigned int m_shaderHandle = 0;
	unsigned int m_type;
	std::string m_src;
	std::string m_path;

public:
	Shader(std::string shaderSource, unsigned int type);
	std::string GetSource();
	void Init();
	Shader(ShaderPath path, unsigned int type);
	const unsigned int GetShaderHandle();
	
};