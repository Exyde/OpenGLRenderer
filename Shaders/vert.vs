#version 330 core

layout (location = 0) in vec3 aPos; 
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;



uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float uTime;

out StagedData {
    float Height;
    vec2 TexCoord;
    vec3 Normal;
    vec3 FragPosWorldSpace;
} outData;


void main()
{
    gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0f);
    outData.TexCoord = vec2(aTexCoord.x, aTexCoord.y);
    outData.Normal = mat3(transpose(inverse(model))) * aNormal; // -- Transpose normal to world space 
    outData.FragPosWorldSpace = vec3(model * vec4(aPos.x, aPos.y, aPos.y, 1.0));
    outData.Height = aPos.y;
    gl_PointSize = gl_Position.z;
}     

