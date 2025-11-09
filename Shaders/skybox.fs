#version 330 core

out vec4 FragColor;
in vec3 TexCoord;
uniform samplerCube skybox;

void main(){

    FragColor = texture(skybox, TexCoord);
    FragColor = vec4(vec3(0), 1.0);
}