#pragma once
#include "ShaderProgram.h"
#include "Camera.h"
class PolygonShaderProgram : public ShaderProgram
{
public:
	PolygonShaderProgram(Camera& cam) :
		ShaderProgram(VertexShader(ShaderPath("shaders/polygon/vertex.glsl")),
						FragmentShader(ShaderPath("shaders/polygon/fragment.glsl"))),
						p_cam(&cam) {};
	virtual void Update(const float& time, const float& dt) override;
	virtual void InitUniforms() override;
private:
	Camera* p_cam;
};