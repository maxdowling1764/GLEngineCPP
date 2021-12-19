#include "Mesh.h"
#include <glad/glad.h>

Mesh::Mesh()
{
    m_vertices = {};
    m_indices = {};
    m_textures = {};
}

Mesh::Mesh(std::vector<Vertex>& verts, std::vector<unsigned int>& indices, std::vector<Texture>& textures)
{
    m_vertices = verts;
    m_indices = indices;
    m_textures = textures;
    init();
}

void Mesh::init()
{
    initBuffers();
    bindBuffers();
    initVertexData();
}

void Mesh::initBuffers()
{
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
}

void Mesh::bindBuffers()
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
}

void Mesh::initVertexData()
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    pushVertexData();
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,Normal));
    
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glBindVertexArray(0);
}

void Mesh::pushVertexData()
{
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_vertices[0], GL_STATIC_DRAW);
}
