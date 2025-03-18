package main

import (
	"bufio"
	"bytes"
	"encoding/json"
	"fmt"
	"log"
	"os"
	"strconv"
	"strings"
)

type ErrCode int

const (
	ErrReadMol ErrCode = iota
	ErrRead3DCoords
	ErrReadBond
)

func (e ErrCode) Error() string {
	switch e {
	case ErrReadMol:
		return "failed to read .mol file"
	case ErrRead3DCoords:
		return "failed to read 3D coordinates"
	case ErrReadBond:
		return "failed to read bond data"
	default:
		return "unknown error"
	}
}

type Atom struct {
	X       float64 `json:"x"`
	Y       float64 `json:"y"`
	Z       float64 `json:"z"`
	Element string  `json:"element"`
}

type Bond struct {
	Atom1    int `json:"atom1"`
	Atom2    int `json:"atom2"`
	BondType int `json:"bond_type"`
}

type Molecule struct {
	AtomCount int    `json:"atom_count"`
	BondCount int    `json:"bond_count"`
	Atoms     []Atom `json:"atoms"`
	Bonds     []Bond `json:"bonds"`
}

func read(p []byte) (*Molecule, error) {
	r := bytes.NewReader(p)
	buf := bufio.NewScanner(r)
	mol := &Molecule{}

	for i := 0; buf.Scan(); i++ {
		text := buf.Text()
		if i < 3 {
			continue
		}
		text = strings.TrimSpace(text)

		// Atom Count and Bond Count
		if i == 3 {
			data := strings.Fields(text)
			if len(data) < 2 {
				return nil, fmt.Errorf("invalid mol init line: %d", i)
			}

			atoms, err := strconv.Atoi(data[0])
			if err != nil {
				return nil, ErrReadMol
			}
			mol.AtomCount = atoms

			bonds, err := strconv.Atoi(data[1])
			if err != nil {
				return nil, ErrReadMol
			}
			mol.BondCount = bonds

			mol.Atoms = make([]Atom, 0, atoms)
			mol.Bonds = make([]Bond, 0, bonds)
			continue
		}

		// Mol Atom Coordinates
		if i >= 4 && i < 4+mol.AtomCount {
			data := strings.Fields(text)
			if len(data) < 4 {
				return nil, fmt.Errorf("invalid atom line: %d", i)
			}
			a := Atom{}
			x, err := strconv.ParseFloat(data[0], 64)
			if err != nil {
				return nil, ErrRead3DCoords
			}
			y, err := strconv.ParseFloat(data[1], 64)
			if err != nil {
				return nil, ErrRead3DCoords
			}
			z, err := strconv.ParseFloat(data[2], 64)
			if err != nil {
				return nil, ErrRead3DCoords
			}
			a.X = x
			a.Y = y
			a.Z = z
			a.Element = data[3]

			mol.Atoms = append(mol.Atoms, a)
			continue
		}

		// Mol Bond
		if i >= 4+mol.AtomCount && i < 4+mol.AtomCount+mol.BondCount {
			data := strings.Fields(text)
			if len(data) < 3 {
				return nil, fmt.Errorf("invalid bond line: %d", i)
			}
			b := Bond{}

			a1, err := strconv.Atoi(data[0])
			if err != nil {
				return nil, ErrReadBond
			}
			a2, err := strconv.Atoi(data[1])
			if err != nil {
				return nil, ErrReadBond
			}
			bondType, err := strconv.Atoi(data[2])
			if err != nil {
				return nil, ErrReadBond
			}
			b.Atom1 = a1
			b.Atom2 = a2
			b.BondType = bondType

			mol.Bonds = append(mol.Bonds, b)
			continue
		}
	}

	if err := buf.Err(); err != nil {
		return nil, fmt.Errorf("scanner error: %v", err)
	}

	return mol, nil
}

func main() {
	f, err := os.ReadFile("data/tmp.mol")
	if err != nil {
		log.Fatalf("failed to open file, %v", err)
	}

	mol, err := read(f)
	if err != nil {
		log.Fatalf("failed to read from file, %v", err)
	}

	jsonData, err := json.MarshalIndent(mol, "", "  ")
	if err != nil {
		log.Fatalf("failed to marshal JSON: %v", err)
	}

	err = os.WriteFile("data/molecule.json", jsonData, 0644)
	if err != nil {
		log.Fatal("failed to write to file")
	}
}
