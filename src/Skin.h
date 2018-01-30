#ifndef __SKIN_H__
#define __SKIN_H__

#include <string>
#include <vector>

#include "Mesh.h"
#include "Matrix4.h"

class Skin
{
protected:
    Mesh m_mesh;
    std::vector<Matrix4> m_bindings;
    std::string m_textureName;
    
public:
    Skin(): m_textureName("none") {}
    void load(std::string filename);
    Mesh& getMesh() {return m_mesh;}
    std::string getTextureName() {return m_textureName;}
    Matrix4& getBinding(int joint) {return m_bindings[joint];}
};

#endif