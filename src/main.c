#include <math.h>
#include <shader.h>
#include <GLFW/glfw3.h>
#include <freetype2/ft2build.h>
#include FT_FREETYPE_H
#include <camera.h>
#include <cube.h>
#include <sphere.h>
#include <cylinder.h>
#include <molecule.h>

const float WIDTH = 800.0f;
const float HEIGHT = 600.0f;

Camera camera;

float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
};

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = (float)xpos;
        lastY = (float)ypos;
        firstMouse = false;
    }

    float xoffset = (float)xpos - lastX;
    float yoffset = (float)lastY - ypos;
    lastX = (float)xpos;
    lastY = (float)ypos;

    camera_processMouseMovement(&camera, xoffset, yoffset, true);
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

int main()
{
    int success;
    char infoLog[512];

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "LearningGL", NULL, NULL);
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

    // Shaders
    Shader *sh = shader_create("static/vertex_shader.glsl", "static/fragment_shader.glsl");
    if (!sh)
    {
        printf("Error on creating shader from file\n");
    }

    // camera init
    camera_create_position(&camera, (vec3){0.0f, 0.0f, 10.0f});

    Atom atoms[3];
    Bond bonds[2];
    vec4 bond_color = {1.0f, 1.0f, 1.0f, 1.0f}; // White for Bond (RGBA)
    vec4 h_color = {1.0f, 0.0f, 0.0f, 1.0f};    // Red for hydrogen (RGBA)
    vec4 o_color = {0.0f, 0.0f, 1.0f, 1.0f};    // Blue for oxygen (RGBA)

    // Initialize the atoms with positions, colors, and radii
    atom_init(&atoms[0], "O", (vec3){0.0f, 0.0f, 0.0f}, o_color, 0.3f); // Oxygen
    atom_init(&atoms[1], "H", (vec3){1.0f, 0.0f, 0.0f}, h_color, 0.2f); // Hydrogen 1
    atom_init(&atoms[2], "H", (vec3){0.0f, 1.0f, 0.0f}, h_color, 0.2f); // Hydrogen 2

    // Init Bonds
    bond_init(&bonds[0], SINGLE_BOND, &atoms[0], &atoms[1], bond_color, 0.05f); // O-H bond 1
    bond_init(&bonds[1], SINGLE_BOND, &atoms[0], &atoms[2], bond_color, 0.05f); // O-H bond 2

    Molecule *water = (Molecule *)malloc(sizeof(Molecule));
    if (!water)
    {
        printf("Error allocating memory to create Molecule\n");
    }
    molecule_init(water, "Water", 3, atoms, 2, bonds);

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
        glClear(GL_COLOR_BUFFER_BIT);

        mat4 view;
        camera_getViewMatrix(&camera, view);

        mat4 projection;
        glm_perspective(glm_rad(camera.zoom), WIDTH / HEIGHT, 0.1f, 100.0f, projection);

        // mol draw
        // TODO: add angle axis, property to sphere/cylinder and control them in theri res draw func fro easy rotation
        molecule_draw(water, sh, view, projection);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    molecule_delete(water);
    free(water);
    shader_delete(sh);
    glfwTerminate();
    return 0;
};
