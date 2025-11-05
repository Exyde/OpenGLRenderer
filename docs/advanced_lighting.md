# Advanced Lighting

[Back to menu](index.md)

- [Advanced Lighting](#advanced-lighting)
  - [Intro : Blinn-Phong](#intro--blinn-phong)
  - [Gamma Correction](#gamma-correction)
  - [Shadows](#shadows)
  - [Normal Mapping](#normal-mapping)
  - [HDR](#hdr)
  - [Bloom](#bloom)
  - [Deferred Shading](#deferred-shading)
  - [SSAO](#ssao)


## Intro : Blinn-Phong
- If angle between view & reflection vector (when computing for specular) is greater than 90°, the dot product become negative. 
- Blinn use the **halfway vector** in the specular computation : the unit vector exactly halfway between the view direction and the light direction
- The closer the halfway vector is to the normal vector, the higher the specular contribution.
  
```glsl
vec3 lightDir   = normalize(lightPos - FragPos);
vec3 viewDir    = normalize(viewPos - FragPos);
vec3 halfwayDir = normalize(lightDir + viewDir);
``` 

Aswell, the shininess part need to be multiplied (2 to 4 times) when using Blinn-Phong to match Phong only visuals results. As a reminder :

```glsl
void main()
{
    [...]
    float spec = 0.0;
    if(blinn)
    {
        vec3 halfwayDir = normalize(lightDir + viewDir);  
        spec = pow(max(dot(normal, halfwayDir), 0.0), 16.0);
    }
    else
    {
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), 8.0);
    }
``` 


## Gamma Correction
## Shadows
## Normal Mapping
## HDR
## Bloom
## Deferred Shading
## SSAO