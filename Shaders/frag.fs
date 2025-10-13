#version 330 core

struct DirLight{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Mat{
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emissive;
    float shininess;
};

struct PointLight{
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

#define NR_POINTS_LIGHTS 4
uniform PointLight pointLights[NR_POINTS_LIGHTS];
uniform DirLight dirLight;

in vec3 ourColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPosWorldSpace;
uniform float uTime;
uniform float T;
uniform vec3 ViewPos;

vec3 CalculateDirLight(DirLight light, vec3 normal, vec3 viewDirection);
vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDirection);
vec3 CalculateEmissive(vec2 uvs);
void main()
{
    vec3 normal = normalize(Normal);
    vec3 viewDirection = normalize(ViewPos - FragPosWorldSpace);
    vec2 scrollingUV = TexCoord + vec2(0, -uTime);
    
    vec3 result = CalculateDirLight(dirLight, normal, viewDirection);

    for (int i = 0; i < NR_POINTS_LIGHTS; i++){
        result += CalculatePointLight(pointLights[i], normal, FragPosWorldSpace, viewDirection);
    }
        
    result += CalculateEmissive(scrollingUV);

    FragColor = vec4(result, 1.0);
}

vec3 CalculateDirLight(DirLight light, vec3 normal, vec3 viewDirection){

    // -- Ambiant
    vec3 ambient = light.ambient * vec3(texture(mat.diffuse, TexCoord));

    // -- Diffuse
    vec3 lightDirection = normalize(light.direction);
    float theta = dot(normal, lightDirection);
    float correctedTheta = max(theta, 0.0);
    vec3 diffuse = light.diffuse * correctedTheta * vec3(texture(mat.diffuse, TexCoord));

    // -- Specular
    vec3 reflectionDirection = reflect(-lightDirection, normal);
    float spec = pow(max(dot(viewDirection, reflectionDirection), 0.0) , mat.shininess);
    vec3 specularSampled = vec3(texture(mat.specular, TexCoord));
    vec3 specular = light.specular * spec * specularSampled;

    return (ambient + diffuse + specular);
}

vec3 CalculateEmissive(vec2 uvs){
    return vec3(texture(mat.emissive, uvs));
}

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDirection){

    vec3 lightDir = normalize(light.position - fragPos);


    // -- Diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    // -- Specular
    vec3 reflectionDir = reflect(-lightDir, normal);

    float spec = pow(max(dot(viewDirection, reflectionDir), 0.0 ), mat.shininess);

    // -- Exponential Attenuation
    float distance = length(light.position - fragPos);
    float Attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    vec3 diffuseTex = vec3(texture(mat.diffuse, TexCoord));
    vec3 ambient = light.ambient * diffuseTex;
    vec3 diffuse = light.diffuse * diff * diffuseTex;
    vec3 specular = light.specular * vec3(texture(mat.specular, TexCoord));

    ambient *= Attenuation;
    diffuse *= Attenuation;
    specular *= Attenuation;

    return vec3(0.0);
    return vec3(diffuse); 
    return vec3(ambient + diffuse + specular);
    return vec3(ambient);
    return vec3(specular);
}

