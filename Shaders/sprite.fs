#version 330 core

out vec4 FragColor;

uniform sampler2D sprite;
in vec2 TexCoord;
uniform vec4 tint;

void main(){
    FragColor = vec4(1.0);
    FragColor = tint * vec4(texture(sprite, TexCoord));
}