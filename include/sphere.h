#ifndef SPHERE_H
#define SPHERE_H

#include <glad/glad.h>
#include <cglm/cglm.h>
#include <shader.h>

#define STACK_COUNT 18
#define SECTOR_COUNT 36
#define SP_PI M_PI

typedef struct
{
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    vec3 position; // center of sphere
    vec3 color;
    float radius;
    float angle;

    float vertices[9 * (SECTOR_COUNT + 1) * (STACK_COUNT + 1)];
    unsigned int indices[6 * (STACK_COUNT - 1) * SECTOR_COUNT];
} Sphere;

void sphere_init(Sphere *sphere, vec3 position, vec3 color, float radius);

void sphere_setAngle(Sphere *sphere, float angle);

void sphere_draw(Sphere *sphere, Shader *sh, mat4 view, mat4 projection);

void sphere_delete(Sphere *sphere);

#endif // SPHERE_H
