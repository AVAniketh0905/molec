#include <cube.h>
#include <string.h>

void cube_gen_vecs(Cube *cube, vec3 color)
{
    const float base_vertices[24] = {
        // Positions only
        -0.5f, -0.5f, -0.5f, // 0
        0.5f, -0.5f, -0.5f,  // 1
        0.5f, 0.5f, -0.5f,   // 2
        -0.5f, 0.5f, -0.5f,  // 3
        -0.5f, -0.5f, 0.5f,  // 4
        0.5f, -0.5f, 0.5f,   // 5
        0.5f, 0.5f, 0.5f,    // 6
        -0.5f, 0.5f, 0.5f    // 7
    };

    const unsigned int base_indices[36] = {
        // Front face
        0, 1, 2, 2, 3, 0,
        // Back face
        4, 5, 6, 6, 7, 4,
        // Left face
        0, 3, 7, 7, 4, 0,
        // Right face
        1, 5, 6, 6, 2, 1,
        // Bottom face
        0, 4, 5, 5, 1, 0,
        // Top face
        3, 2, 6, 6, 7, 3};

    // position (x, y, z) + color (r, g, b)
    for (int i = 0; i < 8; i++)
    {
        int vi = i * 6;                                    // 3 for position + 3 for color
        cube->vertices[vi] = base_vertices[i * 3];         // x
        cube->vertices[vi + 1] = base_vertices[i * 3 + 1]; // y
        cube->vertices[vi + 2] = base_vertices[i * 3 + 2]; // z
        cube->vertices[vi + 3] = color[0];                 // r
        cube->vertices[vi + 4] = color[1];                 // g
        cube->vertices[vi + 5] = color[2];                 // b
    }

    memcpy(cube->indices, base_indices, sizeof(base_indices));
}

void cube_init(Cube *cube, vec3 position, vec3 color)
{
    cube_gen_vecs(cube, color);

    glGenBuffers(1, &cube->VBO);
    glGenBuffers(1, &cube->EBO);

    glGenVertexArrays(1, &cube->VAO);
    glBindVertexArray(cube->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, cube->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube->vertices), cube->vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube->indices), cube->indices, GL_STATIC_DRAW);

    // position(x, y, z)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // color(r,b, g)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
};

void cube_draw(Cube *cube, Shader *sh, mat4 view, mat4 projection)
{
    mat4 model;
    glm_mat4_identity(model);
    glm_translate(model, cube->position);
    // TODO: Change angle
    glm_rotate(model, glm_rad(55.0f), (vec3){0.0f, 1.0f, 0.0f});

    mat4 result;
    // order matters
    glm_mat4_mul(view, model, result);
    glm_mat4_mul(projection, result, result);

    unsigned int transformLoc = glGetUniformLocation(sh->ID, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, (const GLfloat *)result);

    glBindVertexArray(cube->VAO);
    glDrawElements(GL_TRIANGLES, CUBE_INDICES, GL_UNSIGNED_INT, 0);
};

void cube_delete(Cube *cube)
{
    glDeleteVertexArrays(1, &cube->VAO);
    glDeleteBuffers(1, &cube->VBO);
    glDeleteBuffers(1, &cube->EBO);
};