#ifndef CUBE_H
#define CUBE_H

#include <shader.h>
#include <glad/glad.h>
#include <cglm/cglm.h>

#define CUBE_VERTICES 216
#define CUBE_INDICES 36

typedef struct
{
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    vec3 position; // center of cube
    vec3 color;
    float scale;

    // 24 vertices (3 position, 3 color)
    float vertices[CUBE_VERTICES];
    unsigned int indices[CUBE_INDICES];
} Cube;

void cube_init(Cube *cube, vec3 position, vec3 color, float scale);

void cube_draw(Cube *cube, Shader *sh, mat4 view, mat4 projection);

void cube_delete(Cube *cube);

#endif // CUBE_H