#pragma once
#include "Mesh.h"
#include <iostream>
#include "Model.h"
class Quad
{
private:
	Model m_model;
	Mesh m_mesh;
public:
	Quad(glm::vec3 topLeft, glm::vec3 topRight, glm::vec3 bottomLeft, glm::vec3 bottomRight) {

		std::vector<glm::vec3> verts = { topLeft, topRight, bottomLeft, bottomRight};
		std::vector<Texture> textures = {};
		std::vector<unsigned int> indices = 
		{ 
			0,1,2,
			2,1,3
			/*1,5,4,4,5,7,
			0,2,3,3,2,6,
			7,5,6,6,5,3,
			4,7,2,2,7,6,
			1,4,0,0,4,2*/
		};
		m_mesh = Mesh(verts, indices, textures);
		m_model = Model(m_mesh);
	};

	void Render(ShaderProgram& shader);
	void Init(VAOLoader& loader);
};

