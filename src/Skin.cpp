#include "Skin.h"

#include <fstream>
#include <string>
#include <vector>

void Skin::load(std::string filename)
{
    std::fstream file;
    file.open(filename, file.in);
    if (!file.is_open())
        throw std::runtime_error(std::string("Error: failed to open file ") + filename);
    
    std::vector<Mesh::Vertex> vertices;
    std::vector<Mesh::Triangle> triangles;
    
    while (!file.eof())
    {
        std::string line;
        
        file >> line;
        if (file.fail())
            break;
        
        if (line.compare("positions") == 0)
        {
            int numverts;
            file >> numverts;
            vertices.reserve(numverts);
            
            file >> line;
            if (line.compare("{") != 0)
                throw std::runtime_error(std::string("Error: expected { but got ") + line);
            
            for (int i = 0; i < numverts; ++i)
            {
                if (vertices.size() <= i)
                    vertices.push_back({0});
                file >> vertices[i].p;
            }
            
            file >> line;
            if (line.compare("}") != 0)
                throw std::runtime_error(std::string("Error: expected } but got ") + line);
        }
        else if (line.compare("normals") == 0)
        {
            int numverts;
            file >> numverts;
            vertices.reserve(numverts);
            
            file >> line;
            if (line.compare("{") != 0)
                throw std::runtime_error(std::string("Error: expected { but got ") + line);
            
            for (int i = 0; i < numverts; ++i)
            {
                if (vertices.size() <= i)
                    vertices.push_back({0});
                file >> vertices[i].n;
            }
            
            file >> line;
            if (line.compare("}") != 0)
                throw std::runtime_error(std::string("Error: expected } but got ") + line);
        }
        else if (line.compare("texcoords") == 0)
        {
            int numverts;
            file >> numverts;
            vertices.reserve(numverts);
            
            file >> line;
            if (line.compare("{") != 0)
                throw std::runtime_error(std::string("Error: expected { but got ") + line);
            
            for (int i = 0; i < numverts; ++i)
            {
                if (vertices.size() <= i)
                    vertices.push_back({0});
                file >> vertices[i].t;
            }
            
            file >> line;
            if (line.compare("}") != 0)
                throw std::runtime_error(std::string("Error: expected } but got ") + line);
        }
        else if (line.compare("material") == 0)
        {
            // discard material name
            file >> line;
            
            file >> line;
            if (line.compare("{") != 0)
                throw std::runtime_error(std::string("Error: expected { but got ") + line);
            
            file >> line;
            if (line.compare("texture") != 0)
                throw std::runtime_error(std::string("Error: expected texture but got ") + line);
            
            file >> m_textureName;
            
            file >> line;
            if (line.compare("}") != 0)
                throw std::runtime_error(std::string("Error: expected } but got ") + line);
        }
        else if (line.compare("skinweights") == 0)
        {
            int numverts;
            file >> numverts;
            
            file >> line;
            if (line.compare("{") != 0)
                throw std::runtime_error(std::string("Error: expected { but got ") + line);
            
            int numweights = 0;
            for (int i = 0; i < numverts; ++i)
            {
                if (vertices.size() <= i)
                    vertices.push_back({0});
                
                int numattachments;
                file >> numattachments;
                if (numattachments > numweights)
                    numweights = numattachments;
                
                for (int j = 0; j < numattachments; ++j)
                {
                    int index;
                    float weight;
                    file >> index >> weight;
                    vertices[i].w.i[j] = index;
                    vertices[i].w.w[j] = round(weight * 255.0f);
                }
            }
            
            file >> line;
            if (line.compare("}") != 0)
                throw std::runtime_error(std::string("Error: expected } but got ") + line);
        }
        else if (line.compare("triangles") == 0)
        {
            int numtriangles;
            file >> numtriangles;
            triangles.reserve(numtriangles);
            
            file >> line;
            if (line.compare("{") != 0)
                throw std::runtime_error(std::string("Error: expected { but got ") + line);
            
            for (int i = 0; i < numtriangles; ++i)
            {
                if (triangles.size() <= i)
                    triangles.push_back({0});
                file >> triangles[i];
            }
            
            file >> line;
            if (line.compare("}") != 0)
                throw std::runtime_error(std::string("Error: expected } but got ") + line);
        }
        else if (line.compare("bindings") == 0)
        {
            int numjoints;
            file >> numjoints;
            
            file >> line;
            if (line.compare("{") != 0)
                throw std::runtime_error(std::string("Error: expected { but got ") + line);
            
            for (int i = 0; i < numjoints; ++i)
            {
                file >> line;
                if (line.compare("matrix") != 0)
                    throw std::runtime_error(std::string("Error: expected matrix but got ") + line);
                
                file >> line;
                if (line.compare("{") != 0)
                    throw std::runtime_error(std::string("Error: expected { but got ") + line);
                
                Matrix4 matrix = Matrix4().identity();
                float *data = matrix.getPointer();
                
                // use matrix class to read in matrix?
                float value;
                for (int row = 0; row < 4; ++row)
                    for (int col = 0; col < 3; ++col)
                        file >> data[4*row + col];
                
                // Compute the inverse (is the uninverted matrix ever used?)
                matrix.invertAffine();
                
                file >> line;
                if (line.compare("}") != 0)
                    throw std::runtime_error(std::string("Error: expected } but got ") + line);
                
                m_bindings.push_back(matrix);
            }
            
            file >> line;
            if (line.compare("}") != 0)
                throw std::runtime_error(std::string("Error: expected } but got ") + line);
        }
        else
            throw std::runtime_error(std::string("Error: expected attribute type but got ") + line);
    }
    
    file.close();
    
    m_mesh.buffer(vertices, triangles);
}