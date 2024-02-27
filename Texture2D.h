#pragma once
#include <GLAD\glad.h>
#include <vector>
#include "ShaderProgram.h"
class Texture2D
{
	const unsigned int width, height;
	std::vector<GLfloat> data;
	GLfloat* p_data;
	GLuint h_Texture2D = -1;
	const std::string name;
public:
	Texture2D() :Texture2D(256, 256) {};
	Texture2D(const unsigned int& w, unsigned const int& h)
		: width(w), height(h), 
			data(std::vector<GLfloat>(w*h * 4, 0.5f)), 
			name("diffuseTex")
	{
		p_data = data.data();
	};
	Texture2D(const unsigned int& w, const unsigned int& h, const std::string& name)
		: width(w), height(h),
			data(std::vector<GLfloat>(w* h * 4, 0.5f)),
			name(name)
	{
		p_data = data.data();
	};
	void RenderInit(); // On GL Context Init
	void Bind(ShaderProgram* shader, const char* id, GLenum slot);
	void Load();
	void Free();
	void SetValue(const unsigned int& i,
					const unsigned int& j,
					const glm::vec4& value);
	void SetData(std::vector<glm::vec4>& d);
	void SetData(std::vector<glm::vec4>& d, size_t start, size_t end);
	template <typename T>
	void SetData(std::vector<T>& d)
	{
		data.clear();
		for (int i = 0; i < d.size(); i++)
		{
			data.push_back((GLfloat)d[i]);
		}
	}

	
};

