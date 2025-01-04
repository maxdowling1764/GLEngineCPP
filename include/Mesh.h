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
	Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& uv) 
		: position(position), normal(normal), uv(uv) {};
	Vertex() :Vertex(glm::vec3(0), glm::vec3(0), glm::vec2(0)) {};
};

struct Index
{
	unsigned int indices[3];
	Index(const unsigned int& position, const unsigned int& normal, const unsigned int& uv) 
		: indices{position, uv, normal}{};
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
	unsigned int ebo;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;	// Face indices
	std::vector<Index> objIndices;
	std::vector<Texture> textures;

	unsigned int n_vp, n_vn, n_vt;

	bool is_loaded = false;

public:
	unsigned int vao;
	unsigned int vbo;

	Mesh() : vertices({}), objIndices({}), indices({}), textures({}), vao(0), vbo(0), ebo(0) {};

	Mesh(std::vector<Vertex>& vertices) 
		: vertices(vertices), indices(std::vector<unsigned int>()), objIndices({}),
		textures(std::vector<Texture>()),
		vao(0), vbo(0), ebo(0)
	{
		unsigned int i = 0;
		for (const Vertex &vertex : vertices)
		{
			this->indices.push_back(i);
			i++;
		}
	};
	Mesh(std::vector<Vertex>& vertices, std::vector<Index>& objIndices, glm::uvec3 objDims)
		: vertices(vertices), indices(std::vector<unsigned int>()), objIndices(objIndices),
		textures(std::vector<Texture>()),
		n_vp(objDims.x), n_vn(objDims.y), n_vt(objDims.z),
		vao(0), vbo(0), ebo(0)
	{
		unsigned int i = 0;
		for (const Vertex& vertex : vertices)
		{
			this->indices.push_back(i);
			i++;
		}
	};

	Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int> indices)
		: vertices(vertices), indices(indices), objIndices({}),
		textures(std::vector<Texture>()),
		vao(0), vbo(0), ebo(0)
	{
		unsigned int i = 0;
		for (const Vertex& vertex : vertices)
		{
			this->indices.push_back(i);
			i++;
		}
	};

	Mesh(std::vector<glm::vec3>& verts, 
		std::vector<unsigned int>& indices, 
		std::vector<Texture>& textures) : Mesh()
	{
		for (int i = 0; i < verts.size(); i++)
		{
			vertices.push_back(Vertex(verts[i]));
		}
		this->indices = indices;
		this->textures = textures;
	};
	
	void Render(ShaderProgram& shader);
	void Init();
	
	std::vector<Vertex>& Vertices();
	std::vector<unsigned int>& Indices();
	std::vector<Index>& OBJIndices();

	glm::uvec3 OBJDims();
};
