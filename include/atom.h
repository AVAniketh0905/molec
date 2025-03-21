#ifndef ATOM_H
#define ATOM_H

#include <cglm/cglm.h>
#include <string.h>
#include <shader.h>
#include <sphere.h>
#include <cylinder.h>

typedef struct
{
    vec3 color;
    float radius;
} AtomProp;

#define ATOM_PROP(color_r, color_g, color_b, rad) {{color_r, color_g, color_b}, rad};

typedef enum
{
    SINGLE_BOND,
    DOUBLE_BOND,
    TRIPLE_BOND
} BondType;

typedef struct
{
    char symbol[4]; // Atomic symbol (e.g., "H", "O", "Cl")

    vec3 position; // 3D coordinates of the atom (x, y, z)
    vec3 color;    // RGBA color for the atom
    float radius;  // Atomic radius for rendering (e.g., van der Waals radius)
    float angle;

    Sphere *sphere;
} Atom;

typedef struct
{
    BondType type;

    vec3 color;
    float radius;
    float angle;

    Cylinder *cy;
} Bond;

void atom_init(Atom *atom, const char *symbol, vec3 position, vec3 color, float radius);
void atom_setAngle(Atom *atom, float angle);
void atom_draw(Atom *atom, Shader *sh, mat4 view, mat4 projection);
void atom_delete(Atom *atom);

void bond_init(Bond *bond, BondType type, Atom *a1, Atom *a2, vec3 color, float radius);
void bond_setAngle(Bond *bond, float angle);
void bond_draw(Bond *bond, Shader *sh, mat4 view, mat4 projection);
void bond_delete(Bond *bond);

#endif // ATOM_H
