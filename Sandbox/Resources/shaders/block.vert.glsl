#version 460 core

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;   // Position in world space
out vec3 Normal;    // Normal in world space
out vec2 TexCoords;

void main() {
    FragPos = vec3(model * vec4(vPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * vNormal; // Correct normal if model has scaling
    TexCoords = vTexCoords;

    gl_Position = projection * view * vec4(FragPos, 1.0);
}