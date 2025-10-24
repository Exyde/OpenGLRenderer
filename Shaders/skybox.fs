#version 330 core

out vec4 FragColor;
in vec3 TexCoord;
uniform samplerCube skybox;
uniform float uTime;

void main(){

    vec3 coord = TexCoord;
    FragColor = texture(skybox, vec3(coord.x, coord.y, coord.z));
}