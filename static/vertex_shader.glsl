#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;

uniform float offset;

out vec4 vertexColor;

void main()
{
   gl_Position = vec4(aPos.x + offset, aPos.y, aPos.z, 1.0);
   vertexColor = vec4(aCol.x, aCol.y, aCol.z, 1.0);
}