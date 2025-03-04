#ifndef SHADER_H
#define SHADER_H

#include <stdbool.h>
#include <glad/glad.h>

typedef struct
{
    unsigned int ID;
} Shader;

Shader *shader_create(const char *vertexPath, const char *fragmentPath);
void shader_use(const Shader *shader);
void shader_setBool(const Shader *shader, const char *name, bool value);
void shader_setInt(const Shader *shader, const char *name, int value);
void shader_setFloat(const Shader *shader, const char *name, float value);
void shader_delete(Shader *shader);

#endif // SHADER_H
