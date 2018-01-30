#include "Mesh.h"
#include "Vector3.h"
#include <cmath>

Mesh::~Mesh()
{
    if (m_vertexArrayObject)
        glDeleteVertexArrays(1, &m_vertexArrayObject);
    if (m_vertexBuffer)
        glDeleteBuffers(1, &m_vertexBuffer);
    if (m_indexBuffer)
        glDeleteBuffers(1, &m_indexBuffer);
}

void Mesh::buffer(std::vector<Vertex>& vertices, std::vector<Triangle>& triangles)//, ShaderProgram& program)
{
    // Generate vertex array object
    if (!m_vertexArrayObject)
        glGenVertexArrays(1, &m_vertexArrayObject);
    glBindVertexArray(m_vertexArrayObject);
    
    // Buffer the vertex data
    if (!m_vertexBuffer)
        glGenBuffers(1, &m_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    // Enable vertex attribute pointer
    //GLint a_vertex = program.getAttribLocation("a_vertex");
    GLint a_vertex = 0;
    glEnableVertexAttribArray(a_vertex);
    glVertexAttribPointer(a_vertex, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, p)));
    
    // Enable tex coord attribute pointer
    //GLint a_texCoord = program.getAttribLocation("a_texCoord");
    GLint a_texCoord = 1;
    if (a_texCoord >= 0)
    {
        glEnableVertexAttribArray(a_texCoord);
        glVertexAttribPointer(a_texCoord, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, t)));
    }
    
    // Enable normal attribute pointer
    //GLint a_normal = program.getAttribLocation("a_normal");
    GLint a_normal = 2;
    if (a_normal >= 0)
    {
        glEnableVertexAttribArray(a_normal);
        glVertexAttribPointer(a_normal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, n)));
    }
    
    // Enable joint index attribute pointer
    //GLint a_normal = program.getAttribLocation("a_index");
    GLint a_index = 3;
    if (a_index >= 0)
    {
        glEnableVertexAttribArray(a_index);
        glVertexAttribIPointer(a_index, 4, GL_UNSIGNED_BYTE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, w.i)));
    }
    
    // Enable joint weight attribute pointer
    //GLint a_normal = program.getAttribLocation("a_weight");
    GLint a_weight = 4;
    if (a_weight >= 0)
    {
        glEnableVertexAttribArray(a_weight);
        glVertexAttribPointer(a_weight, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, w.w)));
    }

    // Buffer the index data
    if (!m_indexBuffer)
        glGenBuffers(1, &m_indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangles.size() * sizeof(Triangle), triangles.data(), GL_STATIC_DRAW);
    m_numIndices = triangles.size() * 3;

    glBindVertexArray(0);
}

void Mesh::render()
{
    // Bind arrays and send draw command
    glBindVertexArray(m_vertexArrayObject);
    glDrawElements(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}