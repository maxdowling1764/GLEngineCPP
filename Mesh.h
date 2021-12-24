#pragma once
#include <vector>
#include <string>
#include "ShaderProgram.h"
#include <glm/glm.hpp>
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
class Mesh
{
public:
	/*
	* Mesh has ownership of these members
	*/
	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;
	std::vector<Texture> m_textures;
	unsigned int m_vao;
	unsigned int m_vbo;
	/*
	* Default Constructor
	*/
	Mesh() :m_vertices({}), m_indices({}), m_textures({}), m_vao(0), m_vbo(0)
	{

	};

	Mesh(std::vector<glm::vec3>& positions, 
		std::vector<unsigned int>& indices) : Mesh()
	{
		for (int i = 0; i < positions.size(); i++)
		{
			m_vertices.push_back(Vertex(positions[i]));
		}
		m_indices = indices;
		m_textures = {};
	};

	Mesh(std::vector<Vertex>& verts,
		std::vector<unsigned int>& indices) :Mesh()
	{
		m_vertices = verts;
		m_indices = indices;
		m_textures = {};
	}

	Mesh(std::vector<Vertex>& verts, 
		std::vector<unsigned int>& indices, 
		std::vector<Texture>& textures) : Mesh()
	{
		m_vertices = verts;
		m_indices = indices;
		m_textures = textures;
	};
	
	void Render(ShaderProgram& shader);
	void Init();
	unsigned int GetVAOId() { return m_vaoID; };
	void SetVAOId(unsigned int vao) { m_vaoID = vao; };

private:
	/*
	* Need to store a reference to loader instance
	*/

	unsigned int m_vaoID;
	unsigned int ebo;
};
//class GeometryLoader
//{
//public:
//	Enable
//private:
//	std::vector<int> vaos;
//	std::vector<int> vbos;
//};