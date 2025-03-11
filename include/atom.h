#ifndef ATOM_H
#define ATOM_H

#include <cglm/cglm.h>
#include <string.h>
#include <shader.h>
#include <sphere.h> // Include your sphere rendering functions

typedef struct
{
    char symbol[4]; // Atomic symbol (e.g., "H", "O", "Cl")
    vec3 position;  // 3D coordinates of the atom (x, y, z)
    float radius;   // Atomic radius for rendering (e.g., van der Waals radius)
    vec4 color;     // RGBA color for the atom
    Sphere *sphere; // Sphere object for rendering the atom
} Atom;

// Atom functions
void atom_init(Atom *atom, const char *symbol, vec3 position, vec4 color, float radius);
void atom_draw(Atom *atom, Shader *sh, mat4 view, mat4 projection);
void atom_delete(Atom *atom);

#endif // ATOM_H
