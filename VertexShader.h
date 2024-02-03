#pragma once
#include "Shader.h"
#include <vector>

class VertexShader : public Shader
{
public:
	VertexShader(ShaderPath path) : Shader(path, GL_VERTEX_SHADER) {};
};
