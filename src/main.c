#include <stdio.h>
#include <glad/glad.h>
#include "mode.h"
#include "ui.h"

void error_callback(int error, const char *description)
{
    fprintf(stderr, "Error: %s\n", description);
}

int main()
{
    if (!glfwInit())
        return -1;

    GLFWwindow *window = glfwCreateWindow(800, 600, "Molec Editor Mode", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    printf("OpenGL initialized!\n");

    // Initialize GLAD before any OpenGL function calls
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("Failed to initialize GLAD\n");
        return -1;
    }

    // Load font
    loadFont("fonts/arial.ttf");

    glfwSetErrorCallback(error_callback);

    glfwSetKeyCallback(window, key_callback);
    glfwSetCharCallback(window, char_callback);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        renderModeOverlay();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
