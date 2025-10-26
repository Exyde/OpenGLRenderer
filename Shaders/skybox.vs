#version 330 core

layout (location = 0) in vec3 aPos; 

out vec3 TexCoord; // Direction vector here for sampling cubemap correctly.
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoord = aPos;
    vec4 pos = projection * view * vec4(aPos, 1.0);
    gl_Position = pos.xyww; // -- Override Z component to W to that Z = W and W / W = 1.0 
    // this way we encsure that skybox always have depth value of 1.0 and then win on everything
}     

