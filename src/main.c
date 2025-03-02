#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <freetype2/ft2build.h>
#include FT_FREETYPE_H

const char *vertex_shader_src = "#version 330 core\n"
                                "layout (location = 0) in vec3 aPos;\n"
                                "void main()\n"
                                "{\n"
                                "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                "}\0";

const char *fragment_shader_src = "#version 330 core\n"
                                  "out vec4 FragColor;\n"
                                  "void main()\n"
                                  "{\n"
                                  "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                  "}\0";

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
};

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
};

void ErrLog(GLuint program, GLenum pname, int success, char *infoLog)
{
    glGetProgramiv(program, pname, &success);
    if (!success)
    {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        printf("ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n%s\n", infoLog);
    }
};

int main()
{
    int success;
    char infoLog[512];

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "LearningGL", NULL, NULL);
    if (window == NULL)
    {
        printf("Failed to create GLFW window\n");
        return -1;
    };

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("Failed to initialize GLAD\n");
        return -1;
    }

    // Shaders
    // vertex
    unsigned int vertex_shader;
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertex_shader, 1, &vertex_shader_src, NULL);
    glCompileShader(vertex_shader);
    ErrLog(vertex_shader, GL_COMPILE_STATUS, success, infoLog);

    // fragment
    unsigned int fragment_shader;
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragment_shader, 1, &fragment_shader_src, NULL);
    glCompileShader(fragment_shader);
    ErrLog(fragment_shader, GL_COMPILE_STATUS, success, infoLog);

    // shader program
    unsigned int gl_shader_prg;
    gl_shader_prg = glCreateProgram();

    glAttachShader(gl_shader_prg, vertex_shader);
    glAttachShader(gl_shader_prg, fragment_shader);
    glLinkProgram(gl_shader_prg);

    glLinkProgram(gl_shader_prg);
    ErrLog(gl_shader_prg, GL_LINK_STATUS, success, infoLog);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    // drawing pipeline
    float vertices[] = {
        // first tri
        -0.5f,
        -0.5f,
        0.0f,
        -0.5f,
        0.5f,
        0.0f,
        0.5f,
        0.5f,
        0.0f,

        // second tri
        0.5f,
        0.5f,
        0.0f,
        0.5f,
        -0.5f,
        0.0f,
        -0.5f,
        -0.5f,
        0.0f,
    };

    unsigned int VBO, VAO;
    glGenBuffers(1, &VBO);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(gl_shader_prg);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(gl_shader_prg);
    glfwTerminate();
    return 0;
};
