#pragma once
#include "ShaderProgram.h"

class PolygonShaderProgram : public ShaderProgram
{
public:
	PolygonShaderProgram() :
		ShaderProgram(VertexShader(ShaderPath("shaders/polygon/vertex.glsl")),
						FragmentShader(ShaderPath("shaders/polygon/fragment.glsl"))) {};
	virtual void Update(const float& time, const float& dt) override;
	virtual void InitUniforms() override;
};