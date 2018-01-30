#ifndef __SHADERPROGRAM_H__
#define __SHADERPROGRAM_H__

#include <string>
#include <unordered_map>
#include <OpenGL/gl3.h>

class ShaderProgram
{
    typedef std::unordered_map<std::string, GLint> Attributes;

protected:
    GLuint m_vertexShader;
    GLuint m_fragmentShader;
    GLuint m_geometryShader;
    GLuint m_tessControlShader;
    GLuint m_tessEvaluationShader;
    GLuint m_program;
    Attributes m_attributes;
    Attributes m_uniforms;

protected:
    static GLuint loadShader(std::string filename, GLenum shaderType);

public:
    ShaderProgram(): m_vertexShader(0), m_fragmentShader(0), m_geometryShader(0), m_tessControlShader(0),
                     m_tessEvaluationShader(0), m_program(0) {}
    ~ShaderProgram()
    {
        if (m_vertexShader)
            glDeleteShader(m_vertexShader);
        if (m_fragmentShader)
            glDeleteShader(m_fragmentShader);
        if (m_geometryShader)
            glDeleteShader(m_geometryShader);
        if (m_tessControlShader)
            glDeleteShader(m_tessControlShader);
        if (m_tessEvaluationShader)
            glDeleteShader(m_tessEvaluationShader);
        if (m_program)
            glDeleteProgram(m_program);
    }

    void loadVertexShader(std::string filename)
    {
        GLuint shader = loadShader(filename, GL_VERTEX_SHADER);
        glDeleteShader(m_vertexShader); // only replace old shader if compile doesn't throw exception
        m_vertexShader = shader;
    }

    void loadFragmentShader(std::string filename)
    {
        GLuint shader = loadShader(filename, GL_FRAGMENT_SHADER);
        glDeleteShader(m_fragmentShader); // only replace old shader if compile doesn't throw exception
        m_fragmentShader = shader;
    }

    void loadGeometryShader(std::string filename)
    {
        GLuint shader = loadShader(filename, GL_GEOMETRY_SHADER);
        glDeleteShader(m_geometryShader); // only replace old shader if compile doesn't throw exception
        m_geometryShader = shader;
    }

    void loadTessControlShader(std::string filename)
    {
        GLuint shader = loadShader(filename, GL_TESS_CONTROL_SHADER);
        glDeleteShader(m_tessControlShader); // only replace old shader if compile doesn't throw exception
        m_tessControlShader = shader;
    }

    void loadTessEvaluationShader(std::string filename)
    {
        GLuint shader = loadShader(filename, GL_TESS_EVALUATION_SHADER);
        glDeleteShader(m_tessEvaluationShader); // only replace old shader if compile doesn't throw exception
        m_tessEvaluationShader = shader;
    }

    void linkProgram();
    void useProgram() {glUseProgram(m_program);}

    GLint getAttribLocation(const GLchar* name);
    GLint getUniformLocation(const GLchar* name);
};

#endif
