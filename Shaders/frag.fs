#version 330 core

struct Mat{
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emissive;
    float shininess;
};

struct Light{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant; /// k1
    float linear; /// k2
    float quadratic; /// k3
};

out vec4 FragColor;  

uniform Mat mat;
uniform Light light;

in vec3 ourColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPosWorldSpace;
uniform float uTime;
uniform float T;
uniform vec3 ViewPos;

void main()
{

    vec2 scrollingUV = TexCoord + vec2(0, -uTime);
    // -- Ambiant
    vec3 ambient = light.ambient * vec3(texture(mat.diffuse, TexCoord));

    // -- Diffuse
    vec3 normal = normalize(Normal);
    vec3 lightDirection = normalize(light.position - FragPosWorldSpace);
    float theta = dot(normal, lightDirection);
    float correctedTheta = max(theta, 0.0);
    vec3 diffuse = light.diffuse * correctedTheta * vec3(texture(mat.diffuse, TexCoord));

    // -- Specular
    vec3 viewDirection = normalize(ViewPos - FragPosWorldSpace);
    vec3 reflectionDirection = reflect(-lightDirection, normal);
    float spec = pow(max(dot(viewDirection, reflectionDirection), 0.0) , mat.shininess);
    vec3 specularSampled = vec3(texture(mat.specular, TexCoord));
    vec3 specular = light.specular * spec * specularSampled;

    // - Emissive
    vec3 mask = (vec3(1.0) - specularSampled);
    vec3 emissive = vec3(texture(mat.emissive, scrollingUV)) * mask;

    // -- Exponential Attenuation
    float distance = length(light.position - FragPosWorldSpace);
    float Attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    ambient *= Attenuation;
    diffuse *= Attenuation;
    specular *= Attenuation;
        
    vec3 phongLights = (ambient + diffuse + specular + (emissive * T));
    FragColor = vec4(phongLights, 1.0);
}

