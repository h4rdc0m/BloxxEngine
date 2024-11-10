#version 330 core

// Input from Vertex Shader
in vec3 FragPos;
in vec2 TexCoords;
in vec3 Normal;

// Output Color
out vec4 FragColor;

// Uniforms
uniform sampler2D albedo;  // Base color texture
uniform vec3 lightPos;     // Light position
uniform vec3 lightColor;   // Light color
uniform vec3 ambientLight; // Ambient light color
uniform vec3 viewPos;      // Camera position

void main()
{
    // -------------------------
    // 1. Texture Sampling
    // -------------------------

    // Sample the albedo texture for color
    vec3 albedoColor = texture(albedo, TexCoords).rgb;

    // -------------------------
    // 2. Lighting Calculations
    // -------------------------

    // Calculate the normal and light direction
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    // Diffuse lighting (Lambertian)
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Ambient lighting
    vec3 ambient = ambientLight * albedoColor;

    // Final color calculation (diffuse + ambient)
    vec3 finalColor = (ambient + diffuse) * albedoColor;

    // -------------------------
    // 3. Output the final color
    // -------------------------

    FragColor = vec4(finalColor, 1.0);
}
