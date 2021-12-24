#include "Mesh.h"
#include <glad/glad.h>
#include "ShaderProgram.h"

void Mesh::Render(ShaderProgram& shader)
{
    shader.Use();
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
void Mesh::Init()
{
}
