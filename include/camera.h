#ifndef CAMERA_H
#define CAMERA_H

#include <stdio.h>
#include <glad/glad.h>
#include <cglm/cglm.h>

typedef enum
{
    CAM_FORWARD,
    CAM_BACKWARD,
    CAM_LEFT,
    CAM_RIGHT
} Camera_Movement;

typedef struct
{

    vec3 position;
    vec3 front;
    vec3 up;
    vec3 right;
    vec3 worldUp;

    float yaw;
    float pitch;

    float movementSpeed;
    float mouseSensitivity;
    float zoom;
} Camera;

void camera_create(Camera *camera, vec3 position, vec3 up, float yaw, float pitch);
void camera_create_default(Camera *camera);
void camera_create_position(Camera *camera, vec3 position);
void camera_setZoom(Camera *camera, float zoom);
void camera_setSensitivity(Camera *camera, float sensitivity);
void camera_setSpeed(Camera *camera, float speed);
void camera_getViewMatrix(Camera *camera, mat4 view);
void camera_processKeyboard(Camera *camera, Camera_Movement direction, float deltaTime);
void camera_processMouseMovement(Camera *camera, float xoffset, float yoffset, GLboolean constrainPitch);
void camera_processMouseScroll(Camera *camera, float yoffset);

#endif
