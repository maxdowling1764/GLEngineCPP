#pragma once
#include <vector>
#include "Mesh.h"
#include <glad/glad.h>
class VAOLoader
{
private:
	std::vector<unsigned int> vaos;
	std::vector<unsigned int> vbos;
	std::vector<unsigned int> ebos;

	/*
	* Create a VAO and return the id of it
	* Side Effect: push_back id into vaos
	*/
	unsigned int createVAO();

	/*
	* Create a VBO and return the id of it
	* Side Effect: push_back id into vbos
	*/
	unsigned int createVBO(unsigned int target);

	/*
	* Note: Don't forget to unbind vboid from target after uploading
	*/
	template <typename T>
	void uploadToVBO(unsigned int vboid, GLuint target, std::vector<T> data, GLuint usage)
	{
		glBindBuffer(target, vboid);
		glBufferData(target, sizeof(data), &data[0], GL_STATIC_DRAW);
	}

	template<typename T>
	void storeToAttribute(unsigned int attribLocation, std::vector<T> data)
	{
		unsigned int vbo = createVBO(GL_ARRAY_BUFFER);
		uploadToVBO(vbo, GL_ARRAY_BUFFER, data, GL_STATIC_DRAW);
		glVertexAttribPointer(attribLocation, sizeof(T), GL_FLOAT, false, 0, (void*)0);
		glBindVertexArray(0);
	}

	/*
	* Overload of storeToAttribute if vbo is already created
	*/
	template<typename T>
	void storeToAttribute(unsigned int& vbo, unsigned int attribLocation, std::vector<T> data)
	{
		uploadToVBO(vbo, GL_ARRAY_BUFFER, data, GL_STATIC_DRAW);
		glVertexAttribPointer(attribLocation, sizeof(T), GL_FLOAT, false, 0, (void*)0);
		glBindVertexArray(0);
	}
	template<typename T>
	bool loadToVAO(std::vector<T> data, std::vector<GLuint> indices, Mesh& r)
	{
		unsigned int vao = createVAO();
		unsigned int vbo = createVBO(GL_ELEMENT_ARRAY_BUFFER);

		unsigned int ebo;
		glGenBuffers(1, &ebo);
		ebos.push_back(ebo);

		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(T), &data[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
			&indices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(T), (void*)0);
		
		glBindVertexArray(0);
		r.m_vao = vao;
		r.m_vbo = vbo;

		r.SetVAOId(vaos.size() - 1);
		return true;
	}
public:
	bool LoadMesh(Mesh& m)
	{
		return loadToVAO(m.m_vertices, m.m_indices, m);
	}
};

