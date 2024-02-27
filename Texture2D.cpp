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
	Load();
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
void Texture2D::Bind(ShaderProgram* shader, const char* id, GLenum slot)
{
	// Bind the 3D texture to the shader program
	shader->u_Set1i(id, slot);
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
void Texture2D::SetData(std::vector<glm::vec4>& d)
{
	data.clear();
	for (glm::vec4 v : d)
	{
		for (unsigned int i = 0; i < 4; i++)
		{
			data.push_back((GLfloat)v[i]);
		}
	}
}

void Texture2D::SetData(std::vector<glm::vec4>& d, size_t start, size_t end)
{
	data.clear();
	if (end <= d.size() && start < end)
	{
		for (int k = start; k < end; k++)
		{
			for (unsigned int i = 0; i < 4; i++)
			{
				data.push_back((GLfloat)d[k][i]);
			}
		}
	}
}