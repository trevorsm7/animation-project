#ifndef __GLFWRENDER_H__
#define __GLFWRENDER_H__

#include "Renderer.h"
#include "Matrix4.h"
#include "ShaderProgram.h"
#include "Mesh.h"
#include "TGAFile.h"

#include <map>
#include <string>
#include <memory>

class GLFWRenderer : public IRenderer
{
protected:
    ShaderProgram m_meshProgram;
    ShaderProgram m_skinProgram;
    Matrix4 m_modelMatrix;
    Matrix4 m_viewMatrix;
    Matrix4 m_projMatrix;
    Mesh m_cubeMesh;
    std::map<std::string, std::shared_ptr<TGAFile>> m_textures;
    
public:
    virtual void initialize() override;
    
    virtual void setModelMatrix(Matrix4& matrix) override;
    virtual void setViewMatrix(Matrix4& matrix) override;
    virtual void setProjectionMatrix(Matrix4& matrix) override;
    
    virtual void preRender() override;
    virtual void postRender() override;
    
    virtual void drawCube() override;
    virtual void drawMesh(Mesh& mesh) override;
    virtual void drawSkeleton(Skeleton& skeleton) override;
    virtual void drawSkin(Skin& skin, Skeleton& skeleton) override;
};

#endif