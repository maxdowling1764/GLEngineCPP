#pragma once
#include <GLAD\glad.h>
#include <vector>
#include "ShaderProgram.h"
class Texture3D
{
	const int width, height, depth;
	std::vector<GLfloat> data;
	GLfloat* p_data;
	GLuint h_texture3D = -1;

public:
	Texture3D() :Texture3D(256, 256, 256) {};
	Texture3D(const unsigned int& w, unsigned const int& h, const unsigned int& d) 
		: width(w), height(h), depth(d), data(std::vector<GLfloat>(w*h*d*4, 0.5f))
	{
		p_data = data.data();	
	};
	void RenderInit(); // On GL Context Init
	void Bind(ShaderProgram* shader, const char* id, GLenum slot);
	void Load();
	void Free();
	void SetValue(const unsigned int& i, 
					const unsigned int& j, 
					const unsigned int& k, glm::vec4& value);
	template <typename T>
	void SetData(std::vector<T>& d)
	{
		data.clear();
		for (int i = 0; i < d.size(); i++)
		{
			data.push_back((GLfloat)d[i]);
		}
	}
	~Texture3D() {
		if (h_texture3D >= 0)
			Free();
	}
};