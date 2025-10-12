#version 330 core

out vec4 FragColor;  

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D albedo;
uniform sampler2D mask;
uniform float T;
uniform vec4 Tint;

void main()
{
    FragColor = mix(texture(albedo, TexCoord), texture(mask, TexCoord), T) * Tint;
}

