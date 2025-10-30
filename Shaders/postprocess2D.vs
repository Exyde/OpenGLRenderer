#version 330 core

layout (location = 0) in vec4 vertex;

out vec2 TexCoords;

uniform bool chaos;
uniform bool confuse;
uniform bool shake;
uniform float uTime;

void main(){
    gl_Position = vec4(vertex.xy, 0.0, 1.0);
    vec2 tex = vertex.zw;
    TexCoords = tex;

    if (chaos){
        float strength = 0.12;
        vec2 pos = vec2(tex.x +sin(uTime) * strength, tex.y + cos(uTime) * strength);
        TexCoords = pos;
    } else if (confuse){
        TexCoords = vec2(1.0 - tex.x, 1 - tex.y); // -- Revert
    } else{
        TexCoords = tex;
    }

    if (shake){
        float strength = 0.01;
        gl_Position.x += cos(uTime * 10.0) * strength;
        gl_Position.y += cos(uTime* 15.0) * strength;
    }
}