#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Struct to store atomic coordinates and element symbol
typedef struct
{
    char element[3]; // Atom symbol (like "C", "O", "H")
    float x, y, z;   // 3D coordinates
} Atom;

// Struct to hold the molecule with atom count
typedef struct
{
    int atom_count;
    Atom *atoms;
} Molecule;

// Function to print the molecule data
void print_molecule(const Molecule *mol)
{
    printf("Atom count: %d\n", mol->atom_count);
    for (int i = 0; i < mol->atom_count; i++)
    {
        printf("%s\t%.4f\t%.4f\t%.4f\n", mol->atoms[i].element, mol->atoms[i].x, mol->atoms[i].y, mol->atoms[i].z);
    }
}

// Free memory for the molecule
void free_molecule(Molecule *mol)
{
    free(mol->atoms);
}

// API function to generate molecule from SMILES input
Molecule generate_molecule(const char *smiles)
{
    Molecule mol = {0, NULL};
    char command[1024];
    snprintf(command, sizeof(command), "bin\\obabel -:\"%s\" -omol --gen3D", smiles);

    FILE *fp = _popen(command, "r");
    if (fp == NULL)
    {
        perror("Error executing obabel");
        return mol;
    }

    char buffer[256];
    int atom_index = 0;
    int atom_count = 0;
    int atom_info_section = 0;

    // Read the MOL file output line by line
    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        // Detect atom count line (usually the 4th line)
        if (atom_index == 3)
        {
            int atoms, bonds;
            if (sscanf(buffer, "%d %d", &atoms, &bonds) == 2)
            {
                atom_count = atoms;
                mol.atom_count = atom_count;
                mol.atoms = (Atom *)malloc(atom_count * sizeof(Atom));
                if (mol.atoms == NULL)
                {
                    perror("Memory allocation failed");
                    _pclose(fp);
                    return mol;
                }
            }
        }

        // Start parsing atom coordinates after the header
        if (atom_info_section)
        {
            if (atom_index - 4 < atom_count)
            {
                char element[3];
                float x, y, z;
                if (sscanf(buffer, "%10f%10f%10f %2s", &x, &y, &z, element) == 4)
                {
                    strncpy(mol.atoms[atom_index - 4].element, element, 2);
                    mol.atoms[atom_index - 4].element[2] = '\0';
                    mol.atoms[atom_index - 4].x = x;
                    mol.atoms[atom_index - 4].y = y;
                    mol.atoms[atom_index - 4].z = z;
                }
            }
        }

        // Detect where atom information starts (after the 4th line)
        if (atom_index == 3)
        {
            atom_info_section = 1;
        }

        atom_index++;
    }

    _pclose(fp);
    return mol;
}
