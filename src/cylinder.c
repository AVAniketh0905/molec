#include <cylinder.h>

void cylinder_gen_sectors(Cylinder *cylinder)
{
    const float sectorStep = 2 * CY_PI / CY_SECTOR_COUNT;
    int vi = 0;

    // for lateral surface
    for (int j = 0; j <= CY_SECTOR_COUNT; j++)
    {
        float sectorAngle = j * sectorStep;
        float x = cylinder->radius * cos(sectorAngle);
        float y = cylinder->radius * sin(sectorAngle);

        for (int i = 0; i < 2; i++)
        {
            float z = (1 - 2 * i) * (cylinder->height / 2); // Top and bottom of the side

            cylinder->vertices[vi++] = x;
            cylinder->vertices[vi++] = y;
            cylinder->vertices[vi++] = z;

            cylinder->vertices[vi++] = cylinder->color[0];
            cylinder->vertices[vi++] = cylinder->color[1];
            cylinder->vertices[vi++] = cylinder->color[2];

            // Normals (side)
            float lengthInv = -1.0f / cylinder->radius;
            cylinder->vertices[vi++] = x * lengthInv;
            cylinder->vertices[vi++] = y * lengthInv;
            cylinder->vertices[vi++] = 0.0f;
        }
    }
};

void cylinder_gen_indices(Cylinder *cylinder)
{
    int vi = 0;

    for (int k1 = 0; k1 < CY_SECTOR_COUNT; k1++)
    {
        int k2 = k1 + CY_SECTOR_COUNT + 1;
        // top triangle
        cylinder->indices[vi++] = k1;
        cylinder->indices[vi++] = k2;
        cylinder->indices[vi++] = k1 + 1;

        // bottom triangle
        cylinder->indices[vi++] = k1 + 1;
        cylinder->indices[vi++] = k2;
        cylinder->indices[vi++] = k2 + 1;
    };
};

void cylinder_init(Cylinder *cylinder, vec3 position, vec3 direction, vec3 color, float radius, float height)
{
    glm_vec3_copy(position, cylinder->position);
    glm_vec3_copy(direction, cylinder->direction);
    glm_vec3_copy(color, cylinder->color);
    cylinder->radius = radius;
    cylinder->height = height;

    cylinder_gen_sectors(cylinder);
    cylinder_gen_indices(cylinder);

    glGenVertexArrays(1, &cylinder->VAO);
    glGenBuffers(1, &cylinder->VBO);
    glGenBuffers(1, &cylinder->EBO);

    glBindVertexArray(cylinder->VAO);

    if (!cylinder->VAO || !cylinder->VBO || !cylinder->EBO)
    {
        printf("Error generating VAO/VBO/EBO\n");
        return;
    }

    glBindBuffer(GL_ARRAY_BUFFER, cylinder->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cylinder->vertices), cylinder->vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cylinder->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cylinder->indices), cylinder->indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
};

void cylinder_draw(Cylinder *cylinder, Shader *sh, mat4 view, mat4 projection)
{
    glUseProgram(sh->ID);

    mat4 model;
    glm_mat4_identity(model);
    glm_translate(model, cylinder->position);

    vec3 z_axis = {0.0f, 0.0f, 1.0f};
    vec3 rotation_axis;
    glm_vec3_cross(z_axis, cylinder->direction, rotation_axis);

    float angle = acosf(glm_vec3_dot(z_axis, cylinder->direction));
    if (glm_vec3_norm(rotation_axis) > 0.001f) // Avoid zero division
    {
        glm_vec3_normalize(rotation_axis);
        mat4 rotation;
        glm_rotate(model, angle, rotation_axis);
    }

    mat4 result;
    // order matters
    glm_mat4_mul(view, model, result);
    glm_mat4_mul(projection, result, result);

    unsigned int modelLoc = glGetUniformLocation(sh->ID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (const GLfloat *)model);

    unsigned int transformLoc = glGetUniformLocation(sh->ID, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, (const GLfloat *)result);

    glBindVertexArray(cylinder->VAO);
    glDrawElements(GL_TRIANGLES, sizeof(cylinder->indices) / sizeof(unsigned int), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void cylinder_delete(Cylinder *cylinder)
{
    glDeleteVertexArrays(1, &cylinder->VAO);
    glDeleteBuffers(1, &cylinder->VBO);
    glDeleteBuffers(1, &cylinder->EBO);
};