#pragma once
#include "Shader.h"
#include <glad/glad.h>
class FragmentShader : public Shader
{
	public:
		FragmentShader(ShaderPath path) : Shader(path, GL_FRAGMENT_SHADER) {};
		FragmentShader(std::string source) : Shader(source, GL_FRAGMENT_SHADER) {}
};
