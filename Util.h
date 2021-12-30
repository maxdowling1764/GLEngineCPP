#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <string>
std::string read_file(std::string& filepath);
unsigned int load_texture(char* path, int& w, int& h, int& n);
struct GLMeshPtr
{
	unsigned int m_vao, m_vbo, m_ebo;
	GLMeshPtr(unsigned int vao, unsigned int vbo, unsigned int ebo) : m_vao(vao), m_vbo(vbo), m_ebo(ebo) {};
};