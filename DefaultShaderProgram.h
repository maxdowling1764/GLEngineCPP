#pragma once
#include "ShaderProgram.h"

class PolygonShaderProgram : public ShaderProgram
{
public:
	PolygonShaderProgram() :
		ShaderProgram(VertexShader(ShaderPath("shaders/polygon/vertex.glsl")),
						FragmentShader(ShaderPath("shaders/polygon/fragment.glsl"))) {};
};