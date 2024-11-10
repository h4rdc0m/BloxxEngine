#version 330 core

const float PI = 3.14159265359;

struct Material {
    sampler2D albedo;      // Base color texture
    sampler2D rmah;        // r=roughness, g=metallic, b=ao, a=height
    sampler2D normal;      // Normal map
};

struct Light {
    vec3 position;
    vec3 color;            // Light color
    vec3 ambient;
};

in vec3 FragPos;
in vec2 TexCoords;
in vec3 Tangent;
in vec3 Bitangent;
in vec3 Normal;

out vec4 FragColor;

uniform Material material;
uniform Light light;
uniform vec3 viewPos;      // Camera position

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir, float heightScale) {
    // Sample height map
    float height = texture(material.rmah, texCoords).a;

    // Calculate the parallax offset
    float parallaxAmount = height * heightScale;

    // Adjust parallax amount based on view angle to reduce artifacts
    float viewAngle = max(dot(vec3(0.0, 0.0, 1.0), viewDir), 0.0);
    parallaxAmount *= viewAngle;

    // Calculate the offset texture coordinates
    vec2 p = viewDir.xy * parallaxAmount;
    return texCoords - p;
}

void main() {
    // Construct TBN matrix
    mat3 TBN = mat3(normalize(Tangent), normalize(Bitangent), normalize(Normal));

    // View direction in tangent space
    vec3 viewDir = normalize(TBN * (viewPos - FragPos));

    // Adjust texture coordinates using parallax mapping
    float heightScale = 0.005; // Reduced height scale to minimize artifacts
    // vec2 texCoords = ParallaxMapping(TexCoords, viewDir, heightScale);
    vec2 texCoords = TexCoords;

    // Optionally clamp texture coordinates to prevent them from going out of bounds
    // texCoords = clamp(texCoords, 0.0, 1.0);

    // Sample textures with adjusted coordinates
    vec3 albedo = texture(material.albedo, texCoords).rgb;
    float roughness = texture(material.rmah, texCoords).r;
    float metallic = texture(material.rmah, texCoords).g;
    float ao = texture(material.rmah, texCoords).b;

    vec3 normalMap = texture(material.normal, texCoords).rgb;
    normalMap = normalize(normalMap * 2.0 - 1.0); // Transform from [0,1] to [-1,1]
    vec3 N = normalize(TBN * normalMap);

    // View and light directions
    vec3 V = normalize(viewPos - FragPos);
    vec3 L = normalize(light.position - FragPos);
    vec3 H = normalize(V + L); // Halfway vector

    // Calculate reflectance at normal incidence (F0)
    vec3 F0 = vec3(0.04); // Default reflectance for non-metals
    F0 = mix(F0, albedo, metallic); // If metal, use albedo as F0

    // Cook-Torrance BRDF components
    float NdotH = max(dot(N, H), 0.0);
    float roughnessSq = roughness * roughness;
    float NDF_numerator = roughnessSq;
    float NDF_denominator = PI * pow((NdotH * NdotH * (roughnessSq - 1.0) + 1.0), 2.0);
    float NDF = NDF_numerator / NDF_denominator;

    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float k = (roughness + 1.0);
    float k2 = (k * k) / 8.0;
    float G_NL = NdotL / (NdotL * (1.0 - k2) + k2);
    float G_NV = NdotV / (NdotV * (1.0 - k2) + k2);
    float G = G_NL * G_NV;

    float HdotV = max(dot(H, V), 0.0);
    vec3 F = F0 + (1.0 - F0) * pow(1.0 - HdotV, 5.0);

    vec3 numerator = NDF * G * F;
    float denominator = 4.0 * NdotV * NdotL + 0.001;
    vec3 specular = numerator / denominator;

    vec3 kD = (vec3(1.0) - F) * (1.0 - metallic);
    vec3 diffuse = kD * albedo / PI;

    // Combine the results
    vec3 radiance = light.color;
    vec3 ambient = light.ambient * albedo * ao; // Ambient lighting component

    vec3 color = ambient + (diffuse + specular) * radiance * NdotL * ao;

    // Apply gamma correction for sRGB
    color = pow(color, vec3(1.0 / 2.2));

    FragColor = vec4(color, 1.0);
}
