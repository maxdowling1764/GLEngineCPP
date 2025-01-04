#pragma once
#include "Mesh.h"
#include <iostream>
#include "Model.h"
class Quad : public SceneObject
{
private:
	Model m_model;
	Mesh m_mesh;
public:
	Quad(glm::vec3 topLeft, glm::vec3 topRight, glm::vec3 bottomLeft, glm::vec3 bottomRight) {
		glm::vec3 n = glm::cross(bottomRight - bottomLeft, topLeft - bottomLeft);
		glm::vec2 uvs[4] = { glm::vec2(0.0f, 1.0f), glm::vec2(1.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec2(1.0f,0.0f)};
		std::vector<Vertex> verts = { Vertex(topLeft, n, uvs[0]), Vertex(topRight, n, uvs[1]), Vertex(bottomLeft, n, uvs[2]), Vertex(bottomRight, n, uvs[3])};
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
		m_mesh = Mesh(verts, indices);
		m_model = Model(m_mesh);
	};

	void Render(ShaderProgram& shader);
	void Update(float dt);
	void Init(VAOLoader& loader);
};

