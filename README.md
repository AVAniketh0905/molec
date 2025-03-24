# Molec

Molec is a lightweight 3D molecule viewer written in `C` with `OpenGL`. It leverages the OpenBabel library to generate atomic positions and properties, making it a powerful tool for visualizing molecular structures. The project showcases modern graphics rendering techniques for interactive and visually appealing molecule rendering.

## Features

- **OpenGL Rendering (C):** Uses GLFW for window and context management and GLAD for dynamic OpenGL function loading.
- **Cross-Platform Support:** Compatible with both Windows and Linux.
- **Extensible Design:** Easily integrates with additional libraries for enhanced functionality.

## Prerequisites

- **Operating System:** Windows 10 or later, or a Linux distribution.
- **C Compiler:** Visual Studio 2019 (or later) or GCC.
- **Dependencies:** GLFW, GLAD, FreeType, cJSON, KHR, cglm, and OpenBabel.

## Molecular Visualization

- Molecule: Penicillin G (Benzylpenicillin)
- SMILES: CC1(C(=O)NC2=C1SC(C2CO)C(=O)O)C3=CC=CC=C3
- Type: Antibiotic
- Function: Treats bacterial infections

![Pencilin](/data/pencilin.gif)

## Installation and Setup

### 1. Clone the Repository

Clone the project repository to your local machine:

```bash
git clone https://github.com/yourusername/molec.git
cd molec
```

### 2. Install Dependencies

#### GLFW

1. Download the precompiled binaries from the [GLFW Download Page](https://www.glfw.org/download.html).
2. Extract the package and place the headers in `include/GLFW` and libraries in the `lib/` directory.
3. Update your project's include paths and linker settings to reference these files.

#### GLAD

1. Visit the [GLAD Generator](https://glad.dav1d.de/).
2. Select the desired OpenGL version (e.g., 3.3 or higher) and language (C).
3. Download and extract the package, placing the files in `include/glad` and adding the source file to your project.

#### FreeType

1. Ensure the necessary FreeType files (`freetype.dll`, `freetype.lib`) are located in the `lib/` directory.
2. Update your include paths to reference `include/freetype2`.

#### Other Libraries

Ensure the include directories for `cJSON`, `KHR`, and `cglm` are correctly referenced in your project.

#### OpenBabel

OpenBabel is used to compute atomic positions and properties.

- **Windows:**
  - The project currently includes `obabel.exe` in the `bin/` directory.
  - No further installation is required; ensure that the `bin/` folder is added to your system PATH or reference the executable directly from your project.
- **Linux:**
  - OpenBabel is typically available through your distribution’s package manager.

### 3. Build the Project

#### Windows

Compile the project using a command similar to the following:

```bash
gcc -o builds/molec.exe (Get-ChildItem -Path src -Filter *.c | ForEach-Object { $_.FullName }) -I include -I include/freetype2 -L lib -lglfw3 -lopengl32 -lgdi32 -lfreetype
```

#### Linux

Compile the project using:

```bash
gcc -o builds/molec src/*.c -I include -I include/freetype2 -L /usr/lib -lglfw -lGL -lm
```

### 4. Run the Application

Run the compiled executable:

```bash
./builds/molec -"{molecule in SMILES format}"
```

example,

```bash
./builds/molec -"CCO"
```

## Usage

- **Controls:** Press `W`, `A`, `S`, and `D` to move the camera up, left, down, and right, respectively.
- **Zoom:** Use the mouse scroll wheel to zoom in and out.

## Troubleshooting

- **Linker Errors:** Verify that all dependency paths (GLFW, GLAD, etc.) are correctly configured in your project settings.
- **DLL Issues (Windows):** If you encounter missing DLL errors (e.g., GLFW or FreeType DLLs), copy the required files from the `lib/` directory into your executable's folder or add their paths to your system PATH.
- **OpenBabel on Linux:** If `obabel` is not found, ensure it is correctly installed via your package manager and that the executable is in your system PATH.
- **Build Configurations:** Ensure that your build configuration (Debug/Release) matches the versions of your precompiled libraries.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for more information.

## Contact

For issues, questions, or contributions, please contact [adimoolamaniketh@gmail.com] or open an issue in the [GitHub repository](https://github.com/AVAniketh0905/molec).
