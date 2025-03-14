#include <stdio.h>
#include <stdlib.h>
#include <shader.h>
#include <glad/glad.h>

int read_from_file(const char *filePath, char *srcCode, size_t bufferSize)
{
    FILE *src = fopen(filePath, "rb");
    if (!src)
    {
        printf("error on opening file");
        return -1;
    }

    fseek(src, 0, SEEK_END);
    long fileSize = ftell(src);
    rewind(src);

    if (fileSize <= 0 || fileSize >= bufferSize)
    {
        fclose(src);
        printf("Invalid or too large file\n");
        return -1;
    }

    size_t bytesRead = fread(srcCode, 1, fileSize, src);
    srcCode[bytesRead] = '\0'; // Null-terminate

    // printf("DEBUG:%d, %d", bufferSize, fileSize);

    fclose(src);
    return fileSize <= bufferSize ? 0 : -1;
}

Shader *shader_create(const char *vertexPath, const char *fragmentPath)
{
    char *vertexCode = malloc(1024);
    char *fragmentCode = malloc(1024);
    if (!vertexCode || !fragmentCode)
    {
        fprintf(stderr, "Memory allocation failed\n");
        free(vertexCode);
        free(fragmentCode);
        return NULL;
    }

    if (read_from_file(vertexPath, vertexCode, 1024) < 0)
    {
        printf("Error reading vertex shader file\n");
        free(vertexCode);
        free(fragmentCode);
        return NULL;
    }

    if (read_from_file(fragmentPath, fragmentCode, 1024) < 0)
    {
        printf("Error reading fragment shader file");
        free(vertexCode);
        free(fragmentCode);
        return NULL;
    }

    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, (const GLchar *const *)&vertexCode, NULL);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog);
    };

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, (const GLchar *const *)&fragmentCode, NULL);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n", infoLog);
    };

    Shader *sh = malloc(sizeof(Shader));
    if (!sh)
    {
        fprintf(stderr, "Failed to allocate Shader struct\n");
        free(vertexCode);
        free(fragmentCode);
        return NULL;
    }

    sh->ID = glCreateProgram();
    glAttachShader(sh->ID, vertex);
    glAttachShader(sh->ID, fragment);
    glLinkProgram(sh->ID);
    glGetProgramiv(sh->ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(sh->ID, 512, NULL, infoLog);
        printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", infoLog);
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    free(vertexCode);
    free(fragmentCode);
    return sh;
};

void shader_use(const Shader *shader)
{
    glUseProgram(shader->ID);
};

void shader_setBool(const Shader *shader, const char *name, bool value)
{
    glUseProgram(shader->ID);
    glUniform1i(glGetUniformLocation(shader->ID, name), value);
};

void shader_setInt(const Shader *shader, const char *name, int value)
{
    glUseProgram(shader->ID);
    glUniform1i(glGetUniformLocation(shader->ID, name), value);
};

void shader_setFloat(const Shader *shader, const char *name, float value)
{
    glUseProgram(shader->ID);
    glUniform1f(glGetUniformLocation(shader->ID, name), value);
};

void shader_setVec3(const Shader *shader, const char *name, vec3 v)
{
    glUseProgram(shader->ID);
    glUniform3fv(glGetUniformLocation(shader->ID, name), 1, v);
}

void shader_delete(Shader *shader)
{
    if (shader)
    {
        glDeleteProgram(shader->ID);
        free(shader);
    }
};