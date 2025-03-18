#ifndef ATOM_H
#define ATOM_H

#include <cglm/cglm.h>
#include <string.h>
#include <shader.h>
#include <sphere.h>
#include <cylinder.h>

typedef enum
{
    SINGLE_BOND,
    DOUBLE_BOND,
    TRIPLE_BOND
} BondType;

typedef struct
{
    char symbol[4]; // Atomic symbol (e.g., "H", "O", "Cl")
    vec3 position;  // 3D coordinates of the atom (x, y, z)
    float radius;   // Atomic radius for rendering (e.g., van der Waals radius)
    vec3 color;     // RGBA color for the atom
    Sphere *sphere;
} Atom;

typedef struct
{
    BondType type;
    float radius;
    vec3 color;
    Cylinder *cy;
} Bond;

void atom_init(Atom *atom, const char *symbol, vec3 position, vec3 color, float radius);
void atom_rotate(Atom *atom, float angle, vec3 axis);
void atom_draw(Atom *atom, Shader *sh, mat4 view, mat4 projection);
void atom_delete(Atom *atom);

void bond_init(Bond *bond, BondType type, Atom *a1, Atom *a2, vec3 color, float radius);
// void bond_rotate(Bond *bond, float angle, vec3 axis);
void bond_draw(Bond *bond, Shader *sh, mat4 view, mat4 projection);
void bond_delete(Bond *bond);

#endif // ATOM_H
