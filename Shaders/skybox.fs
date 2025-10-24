#version 330 core

out vec4 FragColor;
in vec3 TexCoord;
uniform samplerCube skybox;
uniform float uTime;

void main(){

    FragColor = texture(skybox, TexCoord);
}