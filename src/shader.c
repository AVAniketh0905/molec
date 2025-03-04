#include <stdio.h>
#include <shader.h>
#include <glad/glad.h>

Shader shader_create(const char *vertexPath, const char *fragmentPath)
{
    const char *vertexCode;
    const char *fragmentCode;
    Shader sh = {};
    // TODO:
    // read from disc

    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, vertexCode, NULL);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n", infoLog, "\n");
    };

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, fragmentCode, NULL);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n", infoLog, "\n");
    };

    sh.ID = glCreateProgram();
    glAttachShader(sh.ID, vertex);
    glAttachShader(sh.ID, fragment);
    glLinkProgram(sh.ID);
    glGetProgramiv(sh.ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(sh.ID, 512, NULL, infoLog);
        printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n", infoLog, "\n");
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
    return sh;
};

void shader_use(const Shader *shader)
{
    glUseProgram(shader->ID);
};

void shader_setBool(const Shader *shader, const char *name, bool value)
{
    glUniform1i(glGetUniformLocation(shader->ID, name), value);
};

void shader_setInt(const Shader *shader, const char *name, int value)
{
    glUniform1i(glGetUniformLocation(shader->ID, name), value);
};

void shader_setFloat(const Shader *shader, const char *name, float value)
{
    glUniform1f(glGetUniformLocation(shader->ID, name), value);
};

void shader_delete(Shader *shader)
{
    glDeleteProgram(shader->ID);
};