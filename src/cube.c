#include <cube.h>
#include <string.h>

void cube_gen_vecs(Cube *cube)
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

    // TODO: normals are incorrect
    const float base_normals[18] = {
        // Front face (negative Z)
        0.0f, 0.0f, -1.0f,
        // Back face (positive Z)
        0.0f, 0.0f, 1.0f,
        // Left face (negative X)
        -1.0f, 0.0f, 0.0f,
        // Right face (positive X)
        1.0f, 0.0f, 0.0f,
        // Bottom face (negative Y)
        0.0f, -1.0f, 0.0f,
        // Top face (positive Y)
        0.0f, 1.0f, 0.0f};

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
        int vi = i * 9;                                    // 3 for position + 3 for color
        cube->vertices[vi] = base_vertices[i * 3];         // x
        cube->vertices[vi + 1] = base_vertices[i * 3 + 1]; // y
        cube->vertices[vi + 2] = base_vertices[i * 3 + 2]; // z
        cube->vertices[vi + 3] = cube->color[0];           // r
        cube->vertices[vi + 4] = cube->color[1];           // g
        cube->vertices[vi + 5] = cube->color[2];           // b

        int faceIndex = i / 4;
        // TODO: not working
        cube->vertices[vi + 6] = -base_normals[faceIndex * 3];     // nx
        cube->vertices[vi + 7] = -base_normals[faceIndex * 3 + 1]; // ny
        cube->vertices[vi + 8] = -base_normals[faceIndex * 3 + 2]; // nz
    }

    memcpy(cube->indices, base_indices, sizeof(base_indices));
}

void cube_init(Cube *cube, vec3 position, vec3 color, float scale)
{
    glm_vec3_copy(color, cube->color);
    cube_gen_vecs(cube);
    glm_vec3_copy(position, cube->position);
    cube->scale = scale;

    glGenBuffers(1, &cube->VBO);
    glGenBuffers(1, &cube->EBO);

    glGenVertexArrays(1, &cube->VAO);
    glBindVertexArray(cube->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, cube->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube->vertices), cube->vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube->indices), cube->indices, GL_STATIC_DRAW);

    // position(x, y, z)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // color(r,b, g)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // normals(nx, ny, nz)
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
};

void cube_draw(Cube *cube, Shader *sh, mat4 view, mat4 projection)
{
    glUseProgram(sh->ID);

    mat4 model;
    glm_mat4_identity(model);
    glm_translate(model, cube->position);
    // TODO: Change angle
    glm_scale(model, (vec3){cube->scale, cube->scale, cube->scale});
    glm_rotate(model, glm_rad(55.0f), (vec3){0.0f, 1.0f, 0.0f});

    mat4 result;
    // order matters
    glm_mat4_mul(view, model, result);
    glm_mat4_mul(projection, result, result);

    unsigned int modelLoc = glGetUniformLocation(sh->ID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (const GLfloat *)model);

    unsigned int transformLoc = glGetUniformLocation(sh->ID, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, (const GLfloat *)result);

    glBindVertexArray(cube->VAO);
    glDrawElements(GL_TRIANGLES, CUBE_INDICES, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
};

void cube_delete(Cube *cube)
{
    glDeleteVertexArrays(1, &cube->VAO);
    glDeleteBuffers(1, &cube->VBO);
    glDeleteBuffers(1, &cube->EBO);
};