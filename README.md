# Molec

Molec is a simple 3D molecule viewer in `c` and `opengl`. It reads a molecule file in `xyz` format and displays it in a window.

Heavily relies on the `OpenBabel` library for reading the `xyz` file.

## Compile and Run

### Windows

```bash
gcc -o builds/molec.exe (Get-ChildItem -Path src -Filter *.c | ForEach-Object { $_.FullName }) -I include -I include/freetype2 -L lib -lglfw3 -lopengl32 -lgdi32 -lfreetype
```

Test

```bash
gcc -o builds/molec.exe src/test.c src/glad.c src/cJSON.c src/atom.c src/molecule.c src/sphere.c src/cylinder.c src/parse.c -I include -I include/freetype2 -L lib -lglfw3 -lopengl32 -lgdi32 -lfreetype
```

### Linux

```bash
gcc -o builds/molec src/*.c -I include -I include/freetype2 -L /usr/lib -lglfw -lGL -lm
```

```bash
./builds/molec.exe
```
