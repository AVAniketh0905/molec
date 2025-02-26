#include "mode.h"
#include <string.h>

Mode currentMode = MODE_NORMAL;
char inputBuffer[INPUT_BUFFER_SIZE]={0};
int cursorPosition = 0;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {  // Handle key repeat
        if (key == GLFW_KEY_I) {
            currentMode = MODE_INSERT;
        } else if (key == GLFW_KEY_ESCAPE) {
            currentMode = MODE_NORMAL;
            memset(inputBuffer, 0, INPUT_BUFFER_SIZE);
            cursorPosition = 0;
        } else if (currentMode == MODE_INSERT) {
            int len = strlen(inputBuffer);

            if (key == GLFW_KEY_BACKSPACE && cursorPosition > 0) {
                memmove(&inputBuffer[cursorPosition - 1], &inputBuffer[cursorPosition], len - cursorPosition + 1);
                cursorPosition--;
            } else if (key == GLFW_KEY_DELETE && cursorPosition < len) {
                memmove(&inputBuffer[cursorPosition], &inputBuffer[cursorPosition + 1], len - cursorPosition);
            } else if (key == GLFW_KEY_LEFT && cursorPosition > 0) {
                cursorPosition--;
            } else if (key == GLFW_KEY_RIGHT && cursorPosition < len) {
                cursorPosition++;
            } else if (key == GLFW_KEY_ENTER) {
                // Clear input buffer when Enter is pressed
                memset(inputBuffer, 0, INPUT_BUFFER_SIZE);
                cursorPosition = 0;
            }
        }
    }
}

void char_callback(GLFWwindow* window, unsigned int codepoint) {
    if (currentMode == MODE_INSERT && cursorPosition < INPUT_BUFFER_SIZE - 1) {
        inputBuffer[cursorPosition++] = (char)codepoint;
        inputBuffer[cursorPosition] = '\0';
    }
}