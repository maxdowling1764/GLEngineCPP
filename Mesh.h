#pragma once
#include <vector>
#include <string>
#include "ShaderProgram.h"
#include <glm.hpp>
#include <unordered_map>
struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uv;
	
	Vertex(const glm::vec3& position) : Vertex(position, glm::vec3(0), glm::vec2(0)) {};
	Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& uv) : position(position), normal(normal), uv(uv) {};
	Vertex() :Vertex(glm::vec3(0), glm::vec3(0), glm::vec2(0)) {};
};

struct Index
{
	unsigned int indices[3];
	Index(const unsigned int& position, const unsigned int& normal, const unsigned int& uv) : indices{position, normal, uv}{};
	Index() : Index(0, 0, 0) {};
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
	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;	// Face indices
	std::vector<Texture> m_textures;

	bool is_loaded = false;

public:
	unsigned int m_vao;
	unsigned int m_vbo;

	Mesh() : m_vertices({}), m_indices({}), m_textures({}), m_vao(0), m_vbo(0) {};

	Mesh(std::vector<Vertex>& vertices, std::vector<Index>& indices) 
		: m_vertices(vertices), m_indices(std::vector<unsigned int>()), m_vao(0), m_vbo(0) 
	{
		unsigned int i = 0;
		for (const Vertex &vertex : vertices)
		{
			m_indices.push_back(i);
			i++;
		}
	};

	Mesh(std::vector<glm::vec3>& verts, std::vector<unsigned int>& indices, std::vector<Texture>& textures) : Mesh()
	{
		for (int i = 0; i < verts.size(); i++)
		{
			m_vertices.push_back(Vertex(verts[i]));
		}
		m_indices = indices;
		m_textures = textures;
	};
	
	void Render(ShaderProgram& shader);
	void Init();
	unsigned int GetVAOId() { return m_vaoID; };
	void SetVAOId(unsigned int vao) { m_vaoID = vao; };
	
	const std::vector<Vertex>& Vertices()
	{
		return m_vertices;
	}
	const std::vector<unsigned int>& Indices()
	{
		return m_indices;
	}
};
