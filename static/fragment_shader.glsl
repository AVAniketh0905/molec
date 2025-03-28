#version 330 core

in vec3 Normal;
in vec3 FragPos;
in vec4 vertexColor;

uniform vec3 lightPos;  
uniform vec3 lightColor;

out vec4 FragColor;

void main()
{
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = diff * lightColor;

    vec3 result = ambient + diffuse;
    FragColor = vec4(result, 1.0f) * vertexColor;
}
