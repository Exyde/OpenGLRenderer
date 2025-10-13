#version 330 core

out vec4 FragColor;  
uniform vec4 DiffuseColor;
uniform float DiffuseForce;

void main()
{
    FragColor = DiffuseForce * DiffuseColor;
}

