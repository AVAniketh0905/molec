#include <stdlib.h>
#include <string.h>
#include <molecule.h>
#include <cjson/cJSON.h>

#define JSON_FILE_NAME "data/molecule.json"

Molecule *generate_molecule(const char *molecule_str)
{
    // printf("Generating Molecule from %s\n", molecule_str);
    Molecule *mol = malloc(sizeof(Molecule));
    if (!mol)
    {
        printf("Memory allocation error\n");
        return NULL;
    }

    // copy name
    strncpy(mol->name, molecule_str, sizeof(mol->name));
    mol->name[sizeof(mol->name) - 1] = '\0';

    // run subprocess obabel to generate molecule.mol
    char obabel_cmd[256];
    snprintf(obabel_cmd, sizeof(obabel_cmd), "obabel -:%s --gen3D -omol -O data/molecule.mol", molecule_str);
    if (system(obabel_cmd) != 0)
    {
        printf("Failed to generate molecule.mol with Open Babel\n");
        free(mol);
        return NULL;
    }

    // run go parser/main.go to generate molecule.json
    if (system("go run parser/main.go molecule.mol molecule.json") != 0)
    {
        printf("Failed to generate molecule.json with Go parser\n");
        free(mol);
        return NULL;
    }

    load_molecule_from_JSON(JSON_FILE_NAME, mol);

    // printf("finished loading mol from JSON\n");

    if (mol->atoms == NULL || mol->bonds == NULL)
    {
        printf("failed to create molecule struct\n");
        return NULL;
    }

    // printf("finished gen molecule\n");
    return mol;
};

void molecule_init(Molecule *mol, const char *name, int atom_count, Atom *atoms, int bond_count, Bond *bonds)
{
    strncpy(mol->name, name, sizeof(mol->name) - 1);
    mol->name[sizeof(mol->name) - 1] = '\0';

    mol->atom_count = atom_count;
    mol->bond_count = bond_count;

    mol->atoms = (Atom *)malloc(atom_count * sizeof(Atom));
    for (int i = 0; i < atom_count; ++i)
    {
        mol->atoms[i] = atoms[i];
    }

    mol->bonds = (Bond *)malloc(bond_count * sizeof(Bond));
    for (int i = 0; i < bond_count; ++i)
    {
        mol->bonds[i] = bonds[i];
    }
}

void molecule_draw(Molecule *mol, Shader *sh, mat4 view, mat4 projection)
{
    for (int i = 0; i < mol->bond_count; ++i)
    {
        if (mol->bonds)
        {
            bond_draw(&mol->bonds[i], sh, view, projection);
        }
    }

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

    if (mol->bonds)
    {
        for (int i = 0; i < mol->bond_count; ++i)
        {
            bond_delete(&mol->bonds[i]);
        }
        free(mol->bonds);
        mol->bonds = NULL;
    }
}
