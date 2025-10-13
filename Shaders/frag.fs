#version 330 core

out vec4 FragColor;  

in vec3 ourColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPosWorldSpace;

uniform sampler2D albedo;
uniform sampler2D mask;
uniform float T;
uniform vec3 LightPos;
uniform vec3 ViewPos;
uniform vec4 DiffuseColor;
uniform vec4 AmbiantColor;
uniform vec4 SpecularColor;
uniform vec4 SelfColor;
uniform float DiffuseForce;
uniform float SpecularIntensity;


void main()
{
    // -- Ambiant
    float ambientStrenght = 0.1;
    vec4 ambient = ambientStrenght * AmbiantColor;

    // -- Diffuse
    vec3 normal = normalize(Normal);
    vec3 lightDirection = normalize(LightPos - FragPosWorldSpace);
    float theta = dot(normal, lightDirection);
    float correctedTheta = max(theta, 0.0);
    vec4 diffuse = correctedTheta * DiffuseColor * DiffuseForce;

    // -- Specular
    vec3 viewDirection = normalize(ViewPos - FragPosWorldSpace);
    vec3 reflectionDirection = reflect(-lightDirection, normal);
    float shinyness = 32; 
    float spec = pow(max(dot(viewDirection, reflectionDirection), 0.0) , shinyness);
    vec4 specular = spec * SpecularColor * SpecularIntensity;

    vec4 phongLights = (ambient + diffuse + specular) * SelfColor;

    phongLights.a = 1;
    FragColor = phongLights;
    //FragColor = mix(texture(albedo, TexCoord), texture(mask, TexCoord), T) * LightColor * SelfColor;
}

