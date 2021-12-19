#pragma once
#include "Shader.h"
#include <glad/glad.h>
class FragmentShader : public Shader
{
	public:
		FragmentShader(std::string source) : Shader(source, GL_VERTEX_SHADER) {}
};
