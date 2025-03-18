#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cglm/cglm.h>
#include <cjson/cJSON.h>
#include <molecule.h>

void load_molecule_from_JSON(const char *filename, Molecule *mol)
{
    printf("Starting to load molecule from JSON...\n");
    FILE *fp = fopen(filename, "rb");
    if (!fp)
    {
        printf("Unable to open json file: %s\n", filename);
        return;
    }
    fseek(fp, 0, SEEK_END);
    long len = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char *data = malloc(len + 1);
    if (!data)
    {
        fclose(fp);
        printf("Memory not available\n");
        return;
    }

    if (fread(data, 1, len, fp) != (size_t)len)
    {
        printf("Failed to read file\n");
        free(data);
        fclose(fp);
        return;
    }
    data[len] = '\0';
    fclose(fp);

    cJSON *json = cJSON_Parse(data);
    free(data);
    if (!json)
    {
        printf("Error parsing JSON\n");
        return;
    }

    // Get atom_count and bond_count
    cJSON *atomCountItem = cJSON_GetObjectItem(json, "atom_count");
    cJSON *bondCountItem = cJSON_GetObjectItem(json, "bond_count");
    if (!cJSON_IsNumber(atomCountItem) || !cJSON_IsNumber(bondCountItem))
    {
        printf("Invalid counts in JSON\n");
        cJSON_Delete(json);
        return;
    }
    mol->atom_count = atomCountItem->valueint;
    mol->bond_count = bondCountItem->valueint;

    // Allocate for Atom + Bond Arrays
    mol->atoms = malloc(sizeof(Atom) * mol->atom_count);
    mol->bonds = malloc(sizeof(Bond) * mol->bond_count);
    if (!mol->atoms || !mol->bonds)
    {
        printf("Memory allocation error for atoms or bonds\n");
        cJSON_Delete(json);
        free(mol->atoms);
        free(mol->bonds);
        return;
    }

    // TODO: Set variable values for based on Element
    vec3 defaultAtomColor = {1.0f, 1.0f, 1.0f}; // white
    float defaultAtomRadius = 0.2f;
    vec3 defaultBondColor = {0.5f, 0.5f, 0.5f}; // grayish
    float defaultBondRadius = 0.08f;

    // Atoms Array
    cJSON *atomsArray = cJSON_GetObjectItem(json, "atoms");
    if (!cJSON_IsArray(atomsArray))
    {
        printf("Atoms is not an array\n");
        cJSON_Delete(json);
        free(mol->atoms);
        free(mol->bonds);
        return;
    }

    int i = 0;
    cJSON *atomItem = NULL;
    cJSON_ArrayForEach(atomItem, atomsArray)
    {
        cJSON *xItem = cJSON_GetObjectItem(atomItem, "x");
        cJSON *yItem = cJSON_GetObjectItem(atomItem, "y");
        cJSON *zItem = cJSON_GetObjectItem(atomItem, "z");
        cJSON *elementItem = cJSON_GetObjectItem(atomItem, "element");
        if (!cJSON_IsNumber(xItem) || !cJSON_IsNumber(yItem) ||
            !cJSON_IsNumber(zItem) || !cJSON_IsString(elementItem))
        {
            printf("Invalid atom data\n");
            continue;
        }
        vec3 pos = {(float)xItem->valuedouble, (float)yItem->valuedouble, (float)zItem->valuedouble};

        atom_init(&mol->atoms[i], elementItem->valuestring, pos, defaultAtomColor, defaultAtomRadius);

        i++;
    }

    // Bonds Array
    cJSON *bondsArray = cJSON_GetObjectItem(json, "bonds");
    if (!cJSON_IsArray(bondsArray))
    {
        printf("Bonds is not an array\n");
        cJSON_Delete(json);
        free(mol->atoms);
        free(mol->bonds);
        return;
    }

    i = 0;
    cJSON *bondItem = NULL;
    cJSON_ArrayForEach(bondItem, bondsArray)
    {
        cJSON *atom1Item = cJSON_GetObjectItem(bondItem, "atom1");
        cJSON *atom2Item = cJSON_GetObjectItem(bondItem, "atom2");
        cJSON *bondTypeItem = cJSON_GetObjectItem(bondItem, "bond_type");
        if (!cJSON_IsNumber(atom1Item) || !cJSON_IsNumber(atom2Item) ||
            !cJSON_IsNumber(bondTypeItem))
        {
            printf("Invalid bond data\n");
            continue;
        }
        int idx1 = atom1Item->valueint;
        int idx2 = atom2Item->valueint;
        // 1-based index to 0-based index
        if (idx1 < 1 || idx1 > mol->atom_count || idx2 < 1 || idx2 > mol->atom_count)
        {
            printf("Bond atom index out of range\n");
            continue;
        }
        Atom *a1 = &mol->atoms[idx1 - 1];
        Atom *a2 = &mol->atoms[idx2 - 1];

        // Map bond_type from JSON to your enum
        int bt = bondTypeItem->valueint;
        BondType type;
        if (bt == 1)
            type = SINGLE_BOND;
        else if (bt == 2)
            type = DOUBLE_BOND;
        else if (bt == 3)
            type = TRIPLE_BOND;
        else
            type = SINGLE_BOND; // fallback

        bond_init(&mol->bonds[i], type, a1, a2, defaultBondColor, defaultBondRadius);
        i++;
    }

    cJSON_Delete(json);
    return;
}