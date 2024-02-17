#pragma once
#include "ShaderProgram.h"

class VolumetricShaderProgram : public ShaderProgram 
{
public:
	VolumetricShaderProgram() :
		ShaderProgram(VertexShader(ShaderPath("shaders/volumetric/vertex.glsl")), 
						FragmentShader(ShaderPath("shaders/volumetric/fragment.glsl"))) {};
};