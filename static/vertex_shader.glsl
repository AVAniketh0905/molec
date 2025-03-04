#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;

uniform mat4 transform;

out vec4 vertexColor;

void main()
{
   gl_Position = transform * vec4(aPos.x, aPos.y, aPos.z, 1.0);
   vertexColor = vec4(aCol.x, aCol.y, aCol.z, 1.0);
}