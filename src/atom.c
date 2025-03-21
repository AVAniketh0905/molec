#include <atom.h>

// Atom
void atom_init(Atom *atom, const char *symbol, vec3 position, vec3 color, float radius)
{
    // Copy the symbol (e.g., "H", "O", "Cl")
    strncpy(atom->symbol, symbol, sizeof(atom->symbol) - 1);
    atom->symbol[sizeof(atom->symbol) - 1] = '\0';

    // Set atom properties
    glm_vec3_copy(position, atom->position);
    glm_vec3_copy(color, atom->color);
    atom->radius = radius;

    atom->sphere = (Sphere *)malloc(sizeof(Sphere));
    if (atom->sphere)
    {
        sphere_init(atom->sphere, atom->position, atom->color, atom->radius);
    }
}

void atom_setAngle(Atom *atom, float angle)
{
    if (atom->sphere)
    {
        sphere_setAngle(atom->sphere, angle);
    }
};

void atom_draw(Atom *atom, Shader *sh, mat4 view, mat4 projection)
{
    if (atom->sphere)
    {
        sphere_draw(atom->sphere, sh, view, projection);
    }
};

void atom_delete(Atom *atom)
{
    if (atom->sphere)
    {
        sphere_delete(atom->sphere);
        free(atom->sphere);
    }
};

// Bond
void bond_init(Bond *bond, BondType type, Atom *a1, Atom *a2, vec3 color, float radius)
{
    bond->type = type;
    bond->radius = radius;
    glm_vec3_copy(color, bond->color);

    vec3 position;
    glm_vec3_add(a1->position, a2->position, position);
    glm_vec3_scale(position, 0.5f, position);

    vec3 direction;
    glm_vec3_sub(a2->position, a1->position, direction);
    glm_vec3_normalize(direction);

    float height = glm_vec3_distance(a1->position, a2->position);

    bond->cy = (Cylinder *)malloc(sizeof(Cylinder));
    if (bond->cy)
    {
        cylinder_init(bond->cy, position, direction, bond->color, bond->radius, height);
    }
};

void bond_setAngle(Bond *bond, float angle)
{
    if (bond->cy)
    {
        cylinder_setAngle(bond->cy, angle);
    }
};

void bond_draw(Bond *bond, Shader *sh, mat4 view, mat4 projection)
{
    if (bond->cy)
    {
        vec3 perpendicular;
        vec3 up = {0.0f, 0.0f, 1.0f};

        glm_vec3_cross(bond->cy->direction, up, perpendicular);
        if (glm_vec3_norm(perpendicular) < 0.001f) // Edge case: parallel to Y-axis
        {
            vec3 right = {1.0f, 0.0f, 0.0f};
            glm_vec3_cross(bond->cy->direction, right, perpendicular);
        }
        glm_vec3_normalize(perpendicular);

        for (int i = 0; i < bond->type + 1; i++)
        {
            float offset = 3 * bond->radius * (i - bond->type / 2.0f);
            vec3 offset_vector;
            glm_vec3_scale(perpendicular, offset, offset_vector);

            vec3 original_position;
            glm_vec3_copy(bond->cy->position, original_position);
            glm_vec3_add(original_position, offset_vector, bond->cy->position);

            cylinder_draw(bond->cy, sh, view, projection);

            glm_vec3_copy(original_position, bond->cy->position);
        }
    }
};

void bond_delete(Bond *bond)
{
    if (bond->cy)
    {
        cylinder_delete(bond->cy);
        free(bond->cy);
    }
};