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

const std::vector<Vertex>& Mesh::Vertices()
{
    return vertices;
}

const std::vector<unsigned int>& Mesh::Indices()
{
    return indices;
}