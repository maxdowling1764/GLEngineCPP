#include "VAOLoader.h"
unsigned int VAOLoader::createVAO()
{
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	vaos.push_back(vao);
	return vao;
}

unsigned int VAOLoader::createVBO(unsigned int target)
{
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	vbos.push_back(vbo);
	return vbo;
}
