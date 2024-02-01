#pragma once
#include "Shader.h"

class VertexShader : public Shader
{
public:
	VertexShader(std::string source) : Shader(source, GL_VERTEX_SHADER) {};
	VertexShader(ShaderPath path) : Shader(path, GL_VERTEX_SHADER) {};
};
