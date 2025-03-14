#include <sphere.h>

void sphere_gen_stacks_sectors(Sphere *sphere)
{
    const float sectorStep = 2 * SP_PI / SECTOR_COUNT;
    const float stackStep = SP_PI / STACK_COUNT;
    float lengthInv = -1.0f / sphere->radius;
    float sectorAngle, stackAngle; // (theta, phi)

    int vi = 0;

    for (int i = 0; i <= STACK_COUNT; i++)
    {
        stackAngle = (SP_PI / 2) - i * stackStep;
        float xy = sphere->radius * cos(stackAngle);
        float z = sphere->radius * sin(stackAngle);

        for (int j = 0; j <= SECTOR_COUNT; j++)
        {
            sectorAngle = j * sectorStep;
            float x, y;
            x = xy * cos(sectorAngle);
            y = xy * sin(sectorAngle);

            sphere->vertices[vi++] = x;
            sphere->vertices[vi++] = y;
            sphere->vertices[vi++] = z;

            sphere->vertices[vi++] = sphere->color[0];
            sphere->vertices[vi++] = sphere->color[1];
            sphere->vertices[vi++] = sphere->color[2];

            // normals
            sphere->vertices[vi++] = x * lengthInv;
            sphere->vertices[vi++] = y * lengthInv;
            sphere->vertices[vi++] = z * lengthInv;
        };
    };

    // printf("size of vertices:%d\n", vi);
};

void sphere_gen_indices(Sphere *sphere)
{
    int vi = 0;

    for (int i = 0; i < STACK_COUNT; i++)
    {
        int k1 = i * (SECTOR_COUNT + 1);
        int k2 = k1 + SECTOR_COUNT + 1;

        for (int j = 0; j < SECTOR_COUNT; j++, k1++, k2++)
        {
            if (i != 0)
            {
                sphere->indices[vi++] = k1;
                sphere->indices[vi++] = k2;
                sphere->indices[vi++] = k1 + 1;
            };

            if (i != STACK_COUNT - 1)
            {
                sphere->indices[vi++] = k1 + 1;
                sphere->indices[vi++] = k2;
                sphere->indices[vi++] = k2 + 1;
            };
        };
    };

    // printf("size of indices:%d\n", vi);
};

void sphere_init(Sphere *sphere, vec3 position, vec3 color, float radius)
{
    glm_vec3_copy(position, sphere->position);
    glm_vec3_copy(color, sphere->color);
    sphere->radius = radius;

    sphere_gen_stacks_sectors(sphere);
    sphere_gen_indices(sphere);

    glGenVertexArrays(1, &sphere->VAO);
    glGenBuffers(1, &sphere->VBO);
    glGenBuffers(1, &sphere->EBO);

    glBindVertexArray(sphere->VAO);

    if (!sphere->VAO || !sphere->VBO || !sphere->EBO)
    {
        printf("Error generating VAO/VBO/EBO\n");
        return;
    }

    glBindBuffer(GL_ARRAY_BUFFER, sphere->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(sphere->vertices), sphere->vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sphere->indices), sphere->indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
};

void sphere_draw(Sphere *sphere, Shader *sh, mat4 view, mat4 projection)
{
    glUseProgram(sh->ID);

    mat4 model;
    glm_mat4_identity(model);
    glm_translate(model, sphere->position);

    mat4 result;
    // order matters
    glm_mat4_mul(view, model, result);
    glm_mat4_mul(projection, result, result);

    unsigned int modelLoc = glGetUniformLocation(sh->ID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (const GLfloat *)model);

    unsigned int transformLoc = glGetUniformLocation(sh->ID, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, (const GLfloat *)result);

    glBindVertexArray(sphere->VAO);
    glDrawElements(GL_TRIANGLES, 6 * SECTOR_COUNT * STACK_COUNT, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
};

void sphere_delete(Sphere *sphere)
{
    glDeleteVertexArrays(1, &sphere->VAO);
    glDeleteBuffers(1, &sphere->VBO);
    glDeleteBuffers(1, &sphere->EBO);
};