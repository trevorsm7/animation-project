#ifndef __MESH_H__
#define __MESH_H__

#define GLFW_INCLUDE_GLCOREARB
#include "GLFW/glfw3.h"
#include <vector>
#include <iostream>
#include <cstdint>

#include "ShaderProgram.h"

class Mesh
{
public:
    struct Position {float p[3];} __attribute__((__packed__));
    struct Normal {float n[3];} __attribute__((__packed__));
    struct TexCoord {float t[2];} __attribute__((__packed__));
    struct Weight {uint8_t i[4]; uint8_t w[4];} __attribute__((__packed__));
    //struct Vertex {Position p; TexCoord t; Normal n;} __attribute__((__packed__));
    struct Vertex {Position p; TexCoord t; Normal n; Weight w;} __attribute__((__packed__));
    struct Triangle {int f[3];} __attribute__((__packed__));

protected:
    GLuint m_vertexArrayObject;
    GLuint m_vertexBuffer;
    GLuint m_indexBuffer;
    unsigned m_numIndices;
    
public:
    Mesh(): m_vertexArrayObject(0), m_vertexBuffer(0), m_indexBuffer(0) {}
    ~Mesh();
    void buffer(std::vector<Vertex>& vertices, std::vector<Triangle>& triangles);//, ShaderProgram& program);
    void render(); // should probably extract this into Renderer interface
};

inline std::istream& operator>>(std::istream& in, Mesh::Position& v)
{
    return in >> v.p[0] >> v.p[1] >> v.p[2];
}

inline std::istream& operator>>(std::istream& in, Mesh::Normal& v)
{
    return in >> v.n[0] >> v.n[1] >> v.n[2];
}

inline std::istream& operator>>(std::istream& in, Mesh::TexCoord& v)
{
    return in >> v.t[0] >> v.t[1];
}

inline std::istream& operator>>(std::istream& in, Mesh::Vertex& v)
{
    return in >> v.p >> v.t >> v.n;
}

inline std::istream& operator>>(std::istream& in, Mesh::Triangle& t)
{
    return in >> t.f[0] >> t.f[1] >> t.f[2];
}

#endif