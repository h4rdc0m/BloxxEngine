#version 460 core

layout(location = 0) in vec3 aPos;       // Position attribute
layout(location = 1) in vec3 aNormal;    // Normal attribute
layout(location = 2) in vec2 aTexCoords; // Texture coordinate attribute
layout(location = 3) in vec3 aTangent;   // Tangent attribute
layout(location = 4) in vec3 aBitangent; // Bitangent attribute

out vec3 FragPos;
out vec2 TexCoords;
out vec3 Tangent;
out vec3 Bitangent;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    // Transform vertex position to world space
    FragPos = vec3(model * vec4(aPos, 1.0));

    // Pass texture coordinates to fragment shader
    TexCoords = aTexCoords;

    // Transform normal, tangent, and bitangent vectors
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    Normal = normalize(normalMatrix * aNormal);
    Tangent = normalize(normalMatrix * aTangent);
    Bitangent = normalize(normalMatrix * aBitangent);

    // Compute final vertex position in clip space
    gl_Position = projection * view * vec4(FragPos, 1.0);
}