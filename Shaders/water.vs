#version 330 core

layout (location = 0) in vec3 aPos; 
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPosWorldSpace;
out float Height;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float uTime;

void main()
{
    // --- Base position
    vec3 pos = aPos;

    // --- Simple sin wave deformation
    float waveHeight = 0.05;   // amplitude
    float waveFreq   = 120.0;    // fréquence spatiale
    float waveSpeed  = 20;    // vitesse de défilement

    // Combine plusieurs sinusoïdes pour un effet naturel
    float wave =
        sin(pos.x * waveFreq + uTime * waveSpeed) * 0.5 +
        cos(pos.z * waveFreq * 0.7 + uTime * waveSpeed * 1.2) * 0.5;

    pos.y += wave * waveHeight;

    // --- Transformations classiques
    gl_Position = projection * view * model * vec4(pos, 1.0);
    TexCoord = aTexCoord;
    Normal = mat3(transpose(inverse(model))) * aNormal;
    FragPosWorldSpace = vec3(model * vec4(pos, 1.0));
}     

