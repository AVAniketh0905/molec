# Molec

Molec is a simple 3D molecule viewer in `c` and `opengl`. It reads a molecule file in `xyz` format and displays it in a window.

Heavily relies on the `OpenBabel` library for reading the `xyz` file.

## Compile and Run

```bash
gcc -o builds/molec.exe src/main.c src/glad.c -I include -L lib -lglfw3 -lopengl32 -lgdi32
```

```bash
./builds/molec.exe
```
