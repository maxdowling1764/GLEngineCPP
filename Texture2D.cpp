#include "Texture2D.h"

void Texture2D::RenderInit()
{
	glGenTextures(1, &h_Texture2D);
	glBindTexture(GL_TEXTURE_2D, h_Texture2D);

	// Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Texture2D::Load()
{
	if (p_data != nullptr)
	{
		glBindTexture(GL_TEXTURE_2D, h_Texture2D);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, p_data);
	}
}

void Texture2D::Free()
{
	data.clear();
	glDeleteTextures(1, &h_Texture2D);
}

/*
* Pre: &shader is the most recent instance of ShaderProgram to have called ShaderProgram::Use()
*/
void Texture2D::BindSamplerUniform(ShaderProgram& shader, GLenum slot)
{
	// Bind the 3D texture to the shader program
	shader.u_Set1i(name.c_str(), slot);
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, h_Texture2D);
}

void Texture2D::SetValue(const unsigned int& i, const unsigned int&j, const glm::vec4& value)
{
	if (data.size() == width * height * 4
		&& i < width && j < height)
	{
		unsigned int base = 4 * (j + width * i);
		data[base] = value.x;
		data[base + 1] = value.y;
		data[base + 2] = value.z;
		data[base + 3] = value.w;
	}
}