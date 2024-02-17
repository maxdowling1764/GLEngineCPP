#pragma once
#include "ShaderProgram.h"
#include "Camera.h"

class VolumetricShaderProgram : public ShaderProgram 
{
public:
	VolumetricShaderProgram(Camera& cam) :
		ShaderProgram(VertexShader(ShaderPath("shaders/volumetric/vertex.glsl")), 
						FragmentShader(ShaderPath("shaders/volumetric/fragment.glsl"))),
						p_cam(&cam) {};
	virtual void Update(const float& time, const float& dt) override;
	virtual void InitUniforms() override;
private:
	Camera* p_cam;
};