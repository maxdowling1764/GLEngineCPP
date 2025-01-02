#include "Mesh.h"
#include <glad/glad.h>
#include "ShaderProgram.h"

void Mesh::Render(ShaderProgram& shader)
{
    shader.Use();
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
void Mesh::Init()
{
}

std::vector<Vertex>& Mesh::Vertices()
{
    return vertices;
}

std::vector<unsigned int>& Mesh::Indices()
{
    return indices;
}

std::vector<Index>& Mesh::OBJIndices()
{
    return objIndices;
}

glm::uvec3 Mesh::OBJDims()
{
    return glm::uvec3(n_vp, n_vn, n_vt);
}