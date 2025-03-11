#include <stdlib.h>
#include <string.h>
#include <molecule.h>

void molecule_init(Molecule *mol, const char *name, int atom_count, Atom *atoms)
{
    strncpy(mol->name, name, sizeof(mol->name) - 1);
    mol->name[sizeof(mol->name) - 1] = '\0';

    mol->atom_count = atom_count;

    mol->atoms = (Atom *)malloc(atom_count * sizeof(Atom));
    for (int i = 0; i < atom_count; ++i)
    {
        mol->atoms[i] = atoms[i];
    }
}

void molecule_draw(Molecule *mol, Shader *sh, mat4 view, mat4 projection)
{
    for (int i = 0; i < mol->atom_count; ++i)
    {
        if (mol->atoms)
        {
            atom_draw(&mol->atoms[i], sh, view, projection);
        }
    }
}

void molecule_delete(Molecule *mol)
{
    if (mol->atoms)
    {
        for (int i = 0; i < mol->atom_count; ++i)
        {
            atom_delete(&mol->atoms[i]);
        }
        free(mol->atoms);
        mol->atoms = NULL;
    }
}
