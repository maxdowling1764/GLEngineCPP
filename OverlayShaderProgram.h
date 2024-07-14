#pragma once
#include "ShaderProgram.h"
#include "Camera.h"
class OverlayShaderProgram : public ShaderProgram
{
public:
	OverlayShaderProgram(Camera& cam) :
		ShaderProgram(VertexShader(ShaderPath("shaders/overlay/overlay_vertex.glsl")),
			FragmentShader(ShaderPath("shaders/overlay/overlay_fragment.glsl"))),
		p_cam(&cam) {};
	virtual void Update(const float& time, const float& dt) override;
	virtual void InitUniforms() override;
private:
	Camera* p_cam;
};