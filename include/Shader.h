#pragma once
#include <string>
#include <vector>
#include "Util.h"

typedef struct ShaderPath 
{ 
	std::string path; 
	std::vector<std::string> includes;

	ShaderPath(std::string p, const std::vector<std::string>& i) : includes(i), path(p){}; 
	ShaderPath(std::string p) : path(p), includes(std::vector<std::string>()) {};
} ShaderPath;
class Shader
{
protected:
	ShaderPath m_path;
private:
	unsigned int m_shaderHandle;
	unsigned int m_type;
	std::string m_src;
	

public:
	Shader(ShaderPath& path, unsigned int type) : 
		m_shaderHandle(0), m_path(path), m_type(type), m_src(read_file(path.path)) {};
	
	std::string GetSource();
	void Init();
	
	const unsigned int GetShaderHandle();
	
};
