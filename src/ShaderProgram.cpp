#include "ShaderProgram.h"
#include <iostream>
#include <fstream>

GLuint ShaderProgram::loadShader(std::string filename, GLenum shaderType)
{
    // Open shader source file
    std::fstream file;
    file.open(filename, std::ios::in | std::ios::binary);
    if (!file.is_open())
        throw std::runtime_error(std::string("Failed to open shader ") + filename);

    // Get file length
    file.seekg(0, file.end);
    int length = file.tellg();
    file.seekg(0, file.beg);

    // Copy file into memory
    std::unique_ptr<char[]> buffer(new char [length+1]);
    file.read(buffer.get(), length);
    buffer[length] = 0;
    file.close();

    // Compile the shader file
    GLuint shader = glCreateShader(shaderType);
    char* sourceArray[] = {buffer.get()};
    glShaderSource(shader, 1, sourceArray, NULL);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (success == GL_FALSE)
    {
        // Get the length of the error log
        GLint logLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

        // Get the error log and print
        std::unique_ptr<char[]> errorLog(new char [logLength]);
        glGetShaderInfoLog(shader, logLength, &logLength, errorLog.get());
        std::cout << errorLog.get() << std::endl;

        // Exit with failure
        glDeleteShader(shader);
        throw std::runtime_error(std::string("Failed to compile shader ") + filename);
    }

    return shader;
}

void ShaderProgram::linkProgram()
{
    // Bind some commonly used attributes
    GLuint program = glCreateProgram();

    // Link shaders into program
    glAttachShader(program, m_vertexShader);
    glAttachShader(program, m_fragmentShader);
    if (m_geometryShader)
        glAttachShader(program, m_geometryShader);
    if (m_tessControlShader)
        glAttachShader(program, m_tessControlShader);
    if (m_tessEvaluationShader)
        glAttachShader(program, m_tessEvaluationShader);
    glLinkProgram(program);

    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (success == GL_FALSE)
    {
        // Get the length of the error log
        GLint logLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);

        // Get the error log and print
        std::unique_ptr<char[]> errorLog(new char [logLength]);
        glGetProgramInfoLog(program, logLength, &logLength, errorLog.get());
        std::cout << errorLog.get() << std::endl;

        // Exit with failure
        glDeleteProgram(program);
        throw std::runtime_error("Failed to link program");
    }

    // Replace previously loaded program
    glDeleteProgram(m_program);
    m_program = program;

    // Reset attribute and uniform cache
    m_attributes.clear();
    m_uniforms.clear();
}

GLint ShaderProgram::getAttribLocation(const GLchar* name)
{
    GLint attribute;
    
    auto iterator = m_attributes.find(name);
    if (iterator == m_attributes.end())
    {
        attribute = glGetAttribLocation(m_program, name);
        m_attributes[name] = attribute;
    }
    else
        attribute = iterator->second;
    
    return attribute;
}

GLint ShaderProgram::getUniformLocation(const GLchar* name)
{
    GLint uniform;
    
    auto iterator = m_uniforms.find(name);
    if (iterator == m_uniforms.end())
    {
        uniform = glGetUniformLocation(m_program, name);
        m_uniforms[name] = uniform;
    }
    else
        uniform = iterator->second;
    
    return uniform;
}