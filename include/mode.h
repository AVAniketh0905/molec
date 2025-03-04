#ifndef MODE_H
#define MODE_H

#include <GLFW/glfw3.h>

#define INPUT_BUFFER_SIZE 256

typedef enum
{
    MODE_NORMAL,
    MODE_INSERT
} Mode;

extern Mode currentMode;
extern int cursorPosition;
extern char inputBuffer[INPUT_BUFFER_SIZE];

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
void char_callback(GLFWwindow *windows, unsigned int codepoint);

#endif // MODE_H
