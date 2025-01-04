#pragma once
#include "ShaderProgram.h"
#include "Camera.h"
class FragPositionShader : public ShaderProgram
{
public:
	FragPositionShader(Camera& cam) :
		ShaderProgram(VertexShader(ShaderPath("shaders/polygon/vertex.glsl")),
			FragmentShader(ShaderPath("shaders/polygon/frag_model_position.glsl"))),
		p_cam(&cam) {};
	virtual void Update(const float& time, const float& dt) override;
	virtual void InitUniforms() override;
private:
	Camera* p_cam;
};