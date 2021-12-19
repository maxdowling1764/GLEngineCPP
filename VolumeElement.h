#pragma once
#include "Mesh.h"

class VolumeElement
{
private:
	Mesh m_mesh;
public:
	VolumeElement(glm::vec3 pos, glm::mat3 vecs) {
		std::vector<Vertex> verts = {vecs[0]};
		std::vector<Texture> textures = {};
		std::vector<unsigned int> indices = {};
		m_mesh = Mesh(verts, indices, textures);
	};
};

