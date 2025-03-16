#ifndef MOLECULE_H
#define MOLECULE_H

#include <cglm/cglm.h>
#include <shader.h>
#include <atom.h>

#define Y_AIXS (vec3){0.0f, 1.0f, 0.0f}

typedef struct
{
    char name[64];  // Molecule name (e.g., "Water", "Methane")
    int atom_count; // Number of atoms in the molecule
    int bond_count; // Number of bonds in the molecule
    Atom *atoms;    // Array of atoms
    Bond *bonds;    // Array of bonds
} Molecule;

Molecule *generate_molecule(const char *molecule_str);

void molecule_init(Molecule *mol, const char *name, int atom_count, Atom *atoms, int bond_count, Bond *bonds);
void molecule_draw(Molecule *mol, Shader *sh, mat4 view, mat4 projection);
void molecule_delete(Molecule *mol);

#endif // MOLECULE_H
