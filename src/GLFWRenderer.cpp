#define GLFW_INCLUDE_GLCOREARB
#include "GLFW/glfw3.h"

#include "GLFWRenderer.h"
#include <vector>

void GLFWRenderer::initialize()
{
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1, 0.0, 0.0, 0.0);
    
    m_meshProgram.loadVertexShader("lighting.vsh");
    m_meshProgram.loadFragmentShader("lighting.fsh");
    m_meshProgram.linkProgram();
    
    m_skinProgram.loadVertexShader("lighting_skin.vsh");
    m_skinProgram.loadFragmentShader("lighting.fsh");
    m_skinProgram.linkProgram();
    
    std::vector<Mesh::Vertex> vertices;
    std::vector<Mesh::Triangle> triangles;
    int offset = 0;
    
    // Front face
    for (float y = -0.5f; y <= 0.5f; y += 1.0f)
        for (float x = -0.5f; x <= 0.5f; x += 1.0f)
            vertices.push_back({{x, y, 0.5f}, {0}, {0.0f, 0.0f, 1.0f}});
    triangles.push_back({0 + offset, 1 + offset, 2 + offset});
    triangles.push_back({2 + offset, 1 + offset, 3 + offset});
    offset += 4;
    
    // Back face
    for (float y = -0.5f; y <= 0.5f; y += 1.0f)
        for (float x = 0.5f; x >= -0.5f; x -= 1.0f)
            vertices.push_back({{x, y, -0.5f}, {0}, {0.0f, 0.0f, -1.0f}});
    triangles.push_back({0 + offset, 1 + offset, 2 + offset});
    triangles.push_back({2 + offset, 1 + offset, 3 + offset});
    offset += 4;
    
    // Right face
    for (float y = -0.5f; y <= 0.5f; y += 1.0f)
        for (float z = 0.5f; z >= -0.5f; z -= 1.0f)
            vertices.push_back({{0.5f, y, z}, {0}, {1.0f, 0.0f, 0.0f}});
    triangles.push_back({0 + offset, 1 + offset, 2 + offset});
    triangles.push_back({2 + offset, 1 + offset, 3 + offset});
    offset += 4;
    
    // Left face
    for (float y = -0.5f; y <= 0.5f; y += 1.0f)
        for (float z = -0.5f; z <= 0.5f; z += 1.0f)
            vertices.push_back({{-0.5f, y, z}, {0}, {-1.0f, 0.0f, 0.0f}});
    triangles.push_back({0 + offset, 1 + offset, 2 + offset});
    triangles.push_back({2 + offset, 1 + offset, 3 + offset});
    offset += 4;
    
    // Top face
    for (float z = 0.5f; z >= -0.5f; z -= 1.0f)
        for (float x = -0.5f; x <= 0.5f; x += 1.0f)
            vertices.push_back({{x, 0.5f, z}, {0}, {0.0f, 1.0f, 0.0f}});
    triangles.push_back({0 + offset, 1 + offset, 2 + offset});
    triangles.push_back({2 + offset, 1 + offset, 3 + offset});
    offset += 4;
    
    // Bottom face
    for (float z = -0.5f; z <= 0.5f; z += 1.0f)
        for (float x = -0.5f; x <= 0.5f; x += 1.0f)
            vertices.push_back({{x, -0.5f, z}, {0}, {0.0f, -1.0f, 0.0f}});
    triangles.push_back({0 + offset, 1 + offset, 2 + offset});
    triangles.push_back({2 + offset, 1 + offset, 3 + offset});
    offset += 4;
    
    m_cubeMesh.buffer(vertices, triangles);//, m_program);
}

void GLFWRenderer::setModelMatrix(Matrix4& matrix)
{
    m_modelMatrix = matrix;
}

void GLFWRenderer::setViewMatrix(Matrix4& matrix)
{
    m_viewMatrix = matrix;
}

void GLFWRenderer::setProjectionMatrix(Matrix4& matrix)
{
    m_projMatrix = matrix;
}

void GLFWRenderer::preRender()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLFWRenderer::postRender()
{
    // derp
}

void GLFWRenderer::drawCube()
{
    drawMesh(m_cubeMesh);
}

void GLFWRenderer::drawMesh(Mesh& mesh)
{
    Matrix4 modelView = m_viewMatrix * m_modelMatrix;
    glUniformMatrix4fv(m_meshProgram.getUniformLocation("u_modelView"), 1, GL_FALSE, modelView.getPointer());
    
    mesh.render();
}

void GLFWRenderer::drawSkeleton(Skeleton& skeleton)
{
    // Bind the shader program
    m_meshProgram.useProgram();
    glUniformMatrix4fv(m_meshProgram.getUniformLocation("u_projection"), 1, GL_FALSE, m_projMatrix.getPointer());
    
    int boneCount = skeleton.getBoneCount();
    for (int i = 0; i < boneCount; ++i)
    {
        Matrix4 matrix = skeleton.getWorldMatrix(i) * skeleton.getCubeMatrix(i);
        Matrix4 stack = m_modelMatrix;
        m_modelMatrix *= matrix;
        drawCube();
        m_modelMatrix = stack;
    }
}

void GLFWRenderer::drawSkin(Skin& skin, Skeleton& skeleton)
{
    // Bind the shader program
    m_skinProgram.useProgram();
    Matrix4 modelView = m_viewMatrix * m_modelMatrix;
    glUniformMatrix4fv(m_skinProgram.getUniformLocation("u_projection"), 1, GL_FALSE, m_projMatrix.getPointer());
    glUniformMatrix4fv(m_skinProgram.getUniformLocation("u_modelView"), 1, GL_FALSE, modelView.getPointer());
    glUniform1i(m_skinProgram.getUniformLocation("u_texture"), 0);
    glActiveTexture(GL_TEXTURE0);
    
    // Get texture
    std::string textureName = skin.getTextureName();
    auto it = m_textures.find(textureName);
    if (it != m_textures.end())
    {
        auto texture = it->second;
        texture->bindTexture();
    }
    else
    {
        std::shared_ptr<TGAFile> texture = std::shared_ptr<TGAFile>(new TGAFile());
        if (textureName.compare("none") == 0)
            texture->createImage(1, 1, TGAFile::Color16);
        else
            texture->openFile(textureName);
        texture->createTexture();
        m_textures[textureName] = texture;
    }
    
    int boneCount = skeleton.getBoneCount();
    int location = m_skinProgram.getUniformLocation("u_joints");
    for (int i = 0; i < boneCount; ++i)
    {
        Matrix4 world = skeleton.getWorldMatrix(i) * skin.getBinding(i);
        glUniformMatrix4fv(location + 4*i, 1, GL_FALSE, world.getPointer());
    }
    
    skin.getMesh().render();
}