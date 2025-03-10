#include <camera.h>

#define DEFAULT_POSITION ((vec3){0.0f, 0.0f, 0.0f})
#define DEFAULT_UP ((vec3){0.0f, 1.0f, 0.0f})

const float CAM_YAW = -90.0f;
const float CAM_PITCH = 0.0f;
const float CAM_SPEED = 2.5f;
const float CAM_SENSITIVITY = 0.1f;
const float CAM_ZOOM = 45.0f;

void update_cam_vecs(Camera *camera)
{
    vec3 front;
    front[0] = cos(glm_rad(camera->yaw)) * cos(glm_rad(camera->pitch));
    front[1] = sin(glm_rad(camera->pitch));
    front[2] = sin(glm_rad(camera->yaw)) * cos(glm_rad(camera->pitch));

    glm_normalize(front);
    glm_vec3_copy(front, camera->front);

    // also re-calculate the Right and Up vector
    glm_cross(camera->front, camera->up, camera->right);
    glm_normalize(camera->right); // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.

    glm_cross(camera->right, front, camera->up);
    glm_normalize(camera->up);
};

void camera_create(Camera *camera, vec3 position, vec3 up, float yaw, float pitch)
{
    glm_vec3_copy(position, camera->position);
    glm_vec3_copy(up, camera->up);
    camera->yaw = yaw;
    camera->pitch = pitch;
    camera->movementSpeed = CAM_SPEED;
    camera->mouseSensitivity = CAM_SENSITIVITY;
    camera->zoom = CAM_ZOOM;

    update_cam_vecs(camera);
};

void camera_create_default(Camera *camera)
{
    camera_create(camera, DEFAULT_POSITION, DEFAULT_UP, CAM_YAW, CAM_PITCH);
};

void camera_create_position(Camera *camera, vec3 position)
{
    camera_create(camera, position, DEFAULT_UP, CAM_YAW, CAM_PITCH);
};

void camera_setZoom(Camera *camera, float zoom)
{
    camera->zoom = zoom;
};

void camera_setSensitivity(Camera *camera, float sensitivity)
{
    camera->mouseSensitivity = sensitivity;
};

void camera_setSpeed(Camera *camera, float speed)
{
    camera->movementSpeed = speed;
};

void camera_getViewMatrix(Camera *camera, mat4 view)
{
    vec3 res;
    glm_vec3_add(camera->position, camera->front, res);
    glm_lookat(camera->position, res, camera->up, view);
};

void camera_processKeyboard(Camera *camera, Camera_Movement direction, float deltaTime)
{
    float velocity = camera->movementSpeed * deltaTime;
    if (direction == CAM_FORWARD)
        glm_vec3_muladds(camera->front, velocity, camera->position);
    if (direction == CAM_BACKWARD)
        glm_vec3_muladds(camera->front, -velocity, camera->position);
    if (direction == CAM_LEFT)
        glm_vec3_muladds(camera->right, -velocity, camera->position);
    if (direction == CAM_RIGHT)
        glm_vec3_muladds(camera->right, velocity, camera->position);

    // FPS like
    camera->position[1] = 0;
    update_cam_vecs(camera);
};

void camera_processMouseMovement(Camera *camera, float xoffset, float yoffset, GLboolean constrainPitch)
{
    xoffset *= camera->mouseSensitivity;
    yoffset *= camera->mouseSensitivity;

    camera->yaw += xoffset;
    camera->pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (camera->pitch > 89.0f)
            camera->pitch = 89.0f;
        if (camera->pitch < -89.0f)
            camera->pitch = -89.0f;
    }

    // update Front, Right and Up Vectors using the updated Euler angles
    update_cam_vecs(camera);
};

void camera_processMouseScroll(Camera *camera, float yoffset)
{
    camera->zoom -= (float)yoffset * 2.0f;
    if (camera->zoom < 1.0f)
        camera->zoom = 1.0f;
    if (camera->zoom > 45.0f)
        camera->zoom = 45.0f;

    update_cam_vecs(camera);
};