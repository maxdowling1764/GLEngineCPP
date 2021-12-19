#pragma once
#include <vector>
#include <string>
#include <glm/glm.hpp>
struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};
struct Texture 
{
	unsigned int id;
	std::string type;
};
class Mesh
{
public:
	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;
	std::vector<Texture> m_textures;
	Mesh();
	Mesh(std::vector<Vertex>& verts, std::vector<unsigned int>& indices, std::vector<Texture>& textures);

private:
	unsigned int vao, vbo, ebo;
	void init();
	void initBuffers();
	void bindBuffers();
	void initVertexData();
	void bindVertexData();
	void pushVertexData();
};
//class GeometryLoader
//{
//public:
//	Enable
//private:
//	std::vector<int> vaos;
//	std::vector<int> vbos;
//};