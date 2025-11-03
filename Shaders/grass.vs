#version 330 core

layout (location = 0) in vec3 aPos; 
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPosWorldSpace;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float uTime;

void main()
{
    float modY = gl_VertexID % 0;
    float modX = gl_VertexID % 0;
    gl_Position = projection * view * model * vec4(aPos.x + (sin(uTime) * modX * 0.005), aPos.y + (sin(uTime) * modY* 0.1), aPos.z, 1.0f);
    TexCoord = vec2(aTexCoord.x, aTexCoord.y);
    Normal = mat3(transpose(inverse(model))) * aNormal; // -- Transpose normal to world space 
    FragPosWorldSpace = vec3(model * vec4(aPos.x, aPos.y, aPos.y, 1.0));
}     

