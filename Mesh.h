#pragma once
#include <vector>
#include <string>
#include "ShaderProgram.h"
#include <glm.hpp>
struct Vertex
{
	glm::vec3 Position;

	Vertex(glm::vec3 pos)
	{
		Position = pos;
	}
	Vertex() :Vertex(glm::vec3(0.0f)) {};
};
struct Texture 
{
	unsigned int id;
	std::string type;
};

/*
* Wrapper class for OpenGL objects representing a mesh
*/
class Mesh
{
private:
	unsigned int m_vaoID;
	unsigned int ebo;
public:
	std::vector<glm::vec3> m_vertices;
	std::vector<unsigned int> m_indices;	// Face indices
	std::vector<Texture> m_textures;
	unsigned int m_vao;
	unsigned int m_vbo;

	Mesh() :m_vertices({}), m_indices({}), m_textures({}), m_vao(0), m_vbo(0) {};

	Mesh(std::vector<glm::vec3>& verts,
		std::vector<unsigned int>& indices) :Mesh()
	{
		m_vertices = verts;
		m_indices = indices;
		m_textures = {};
	}

	Mesh(std::vector<glm::vec3>& verts, std::vector<unsigned int>& indices, std::vector<Texture>& textures) : Mesh()
	{
		m_vertices = verts;
		m_indices = indices;
		m_textures = textures;
	};
	
	void Render(ShaderProgram& shader);
	void Init();
	unsigned int GetVAOId() { return m_vaoID; };
	void SetVAOId(unsigned int vao) { m_vaoID = vao; };
};
