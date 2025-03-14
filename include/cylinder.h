#ifndef CYLINDER_H
#define CYLINDER_H

#include <glad/glad.h>
#include <cglm/cglm.h>
#include <shader.h>

#define CY_SECTOR_COUNT 36
#define CY_PI M_PI

typedef struct
{
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    vec3 position; // center of sphere
    vec3 direction;
    vec3 color;
    float radius;
    float height;

    float vertices[12 * (CY_SECTOR_COUNT + 1)];
    unsigned int indices[6 * CY_SECTOR_COUNT];
} Cylinder;

void cylinder_init(Cylinder *cylinder, vec3 position, vec3 direction, vec3 color, float radius, float height);

void cylinder_draw(Cylinder *cylinder, Shader *sh, mat4 view, mat4 projection);

void cylinder_delete(Cylinder *cylinder);

#endif // CYLINDER_H