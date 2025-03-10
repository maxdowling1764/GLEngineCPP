#include "Texture3D.h"

void configure(GLuint& textureUnit)
{
	glGenTextures(1, &textureUnit);
	glBindTexture(GL_TEXTURE_3D, textureUnit);

	// Set texture parameters
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
void Texture3D::RenderInit()
{
	configure(h_texture3D);
	Load();
}

void Texture3D::Load()
{
	glBindTexture(GL_TEXTURE_3D, h_texture3D);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, width, height, depth, 0, GL_RGBA, GL_FLOAT, p_data);
	//data.clear();
}

void Texture3D::Free()
{
	data.clear();
	glDeleteTextures(1, &h_texture3D);
}

/*
* Pre: &shader is the most recent instance of ShaderProgram to have called ShaderProgram::Use()
*/
void Texture3D::Bind(ShaderProgram* shader, const char* textureName, GLenum slot)
{
	// Bind the 3D texture to the shader program
	shader->u_Set1i(textureName, 0);
	glActiveTexture(slot);
	glBindTexture(GL_TEXTURE_3D, h_texture3D);
}

void Texture3D::SetValue(const unsigned int& i, const unsigned int& j, const unsigned int& k, glm::vec4& value)
{
	if (data.size() == width*height*depth*4 
		&& i < width && j < height && k < depth)
	{
		unsigned int base = 4*(k + width*(i + height * j));
		data[base] = value.x;
		data[base + 1] = value.y;
		data[base + 2] = value.z;
		data[base + 3] = value.w;
	}
}

void Texture3D::SetData(std::vector<glm::vec4>& d)
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

void Texture3D::SetData(std::vector<glm::vec4>& d, size_t start, size_t end)
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