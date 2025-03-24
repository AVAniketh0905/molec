#include <math.h>
#include <shader.h>
#include <GLFW/glfw3.h>
#include <freetype2/ft2build.h>
#include FT_FREETYPE_H
#include <camera.h>
#include <ui.h>
#include <cube.h>
#include <sphere.h>
#include <cylinder.h>
#include <molecule.h>

const float WIDTH = 800.0f;
const float HEIGHT = 600.0f;

Camera camera;

float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
};

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
};

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    camera_processMouseScroll(&camera, (float)yoffset);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera_processKeyboard(&camera, CAM_FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera_processKeyboard(&camera, CAM_BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera_processKeyboard(&camera, CAM_LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera_processKeyboard(&camera, CAM_RIGHT, deltaTime);
    }
};

static void ErrLog(GLuint program, GLenum pname, int success, char *infoLog)
{
    glGetProgramiv(program, pname, &success);
    if (!success)
    {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        printf("ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n%s\n", infoLog);
    }
};

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s -\"<molecule_string>\"\n", argv[0]);
        return 1;
    }

    // Check if the argument starts with a dash
    if (argv[1][0] != '-')
    {
        printf("Error: Argument must start with a dash ('-').\n");
        return 1;
    }

    char *mol_str = argv[1] + 1;

    // Check if the molecule string is not empty
    if (strlen(mol_str) == 0)
    {
        printf("Error: Molecule string is empty.\n");
        return 1;
    }

    printf("Molecule string: %s\n", mol_str);

    int success;
    char infoLog[512];

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "MolecGL", NULL, NULL);
    if (window == NULL)
    {
        printf("Failed to create GLFW window\n");
        return -1;
    };

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // glfwSetCursorPosCallback(window, mouse_callback);
    // glfwSetScrollCallback(window, scroll_callback);

    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("Failed to initialize GLAD\n");
        return -1;
    }

    glfwSwapInterval(1);
    glEnable(GL_DEPTH_TEST); // Enable depth testing

    // Shaders
    Shader *sh = shader_create("static/vertex_shader.glsl", "static/fragment_shader.glsl");
    if (!sh)
    {
        printf("Error on creating shader from file\n");
    }

    Shader *light_sh = shader_create("static/light_vs.glsl", "static/light_fs.glsl");
    if (!light_sh)
    {
        printf("Error on creating light shader from file\n");
    }

    // Text shader
    Shader *text_sh = shader_create("static/text_vs.glsl", "static/text_fs.glsl");
    if (!text_sh)
    {
        printf("Error on creating text shader from file\n");
    }

    // load font
    loadFont("fonts/arial.ttf");

    // camera init
    camera_create_position(&camera, (vec3){0.0f, 0.0f, 10.0f});

    // gen molecule
    Molecule *mol = generate_molecule(mol_str);

    // cube
    Cube *light = (Cube *)malloc(sizeof(Cube));
    cube_init(light, (vec3){0.0f, 0.0f, -10.0f}, (vec3){1.0f, 1.0f, 1.0f}, 1.0f);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mat4 view;
        camera_getViewMatrix(&camera, view);

        mat4 projection;
        glm_perspective(glm_rad(camera.zoom), WIDTH / HEIGHT, 0.1f, 100.0f, projection);

        // cube_draw(light, light_sh, view, projection);

        shader_setVec3(sh, "lightPos", light->position);
        shader_setVec3(sh, "lightColor", light->color);

        // rotate mol
        molecule_setAngle(mol, 10 * glfwGetTime());
        molecule_draw(mol, sh, view, projection);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    cube_delete(light);
    free(light);

    molecule_delete(mol);
    free(mol);

    shader_delete(text_sh);
    shader_delete(light_sh);
    shader_delete(sh);

    glfwTerminate();
    return 0;
};