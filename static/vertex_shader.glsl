#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aColor;

uniform mat4 transform;

out vec4 vertexColor;

void main()
{
   gl_Position = transform * vec4(aPos.x, aPos.y, aPos.z, 1.0);
   vertexColor = vec4(aColor.x, aColor.y, 0.0f, 1.0);
}