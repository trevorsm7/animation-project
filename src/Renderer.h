#ifndef __RENDER_H__
#define __RENDER_H__

#include "Matrix4.h"
#include "Mesh.h"
#include "Skin.h"
#include "Skeleton.h"

class IRenderer
{
public:
    virtual void initialize() = 0;
    
    virtual void setModelMatrix(Matrix4& matrix) = 0;
    virtual void setViewMatrix(Matrix4& matrix) = 0;
    virtual void setProjectionMatrix(Matrix4& matrix) = 0;
    
    virtual void preRender() = 0;
    virtual void postRender() = 0;
    
    virtual void drawCube() = 0;
    virtual void drawMesh(Mesh& mesh) = 0;
    virtual void drawSkeleton(Skeleton& skeleton) = 0;
    virtual void drawSkin(Skin& skin, Skeleton& skeleton) = 0;
};

#endif