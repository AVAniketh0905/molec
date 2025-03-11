#include <atom.h>

void atom_init(Atom *atom, const char *symbol, vec3 position, vec4 color, float radius)
{
    // Copy the symbol (e.g., "H", "O", "Cl")
    strncpy(atom->symbol, symbol, sizeof(atom->symbol) - 1);
    atom->symbol[sizeof(atom->symbol) - 1] = '\0';

    // Set atom properties
    glm_vec3_copy(position, atom->position);
    glm_vec4_copy(color, atom->color);
    atom->radius = radius;

    atom->sphere = (Sphere *)malloc(sizeof(Sphere));
    if (atom->sphere)
    {
        sphere_init(atom->sphere, atom->position, atom->color, atom->radius);
    }
}

void atom_draw(Atom *atom, Shader *sh, mat4 view, mat4 projection)
{
    if (atom->sphere)
    {
        sphere_draw(atom->sphere, sh, view, projection);
    }
}

void atom_delete(Atom *atom)
{
    if (atom->sphere)
    {
        sphere_delete(atom->sphere);
        free(atom->sphere);
        atom->sphere = NULL;
    }
}
