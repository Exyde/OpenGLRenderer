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

- **Gamma** : the exponential relationship between the input voltage and the outpute brightness (roughly 2.2, CRT Monitor)
- There's a big difference between Physical Linear Brightness and actually perceived brightness. 
- The thing is : everything looks better with Gamma Correction
- Read this as some point, it's complete : [What every programmer should know about gamma correction](https://blog.johnnovak.net/2016/09/21/what-every-coder-should-know-about-gamma/)
- The idea is to take a color, apply the gamma correction curve by power it to 1/2.2 (2.2 is standard gamma, let user tweak it), and then the monitor apply the opposite by raising it to 2.2 (or actually the monitor value, that's why we tweak it in settings).

```c++
// As easy as :
glEnable(GL_FRAMEBUFFER_SRGB);
``` 

or in each concerned fragment shader, with more control ( Or actually in post-process pass !)
```glsl
float gamma = 2.2; // or anything user-set, from uniform.
OutputColor.rgb = pow(OutputColor.rgb, vec3(1.0/gamma));
``` 
___
For **Textures**, they are mostly edited in sRGB space since application and artists are mostly used to this. 
We can correct it in shader, or using again a neat openGl command.
Often, albedo/diffuse maps are in sRGB, while specular and normals maps are in linear space, so...

```c++
// sRGB and sRGBA 
glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
``` 

Or correct it manually in shader. 
```glsl
float gamma = 2.2; // or anything user-set, from uniform.
vec3 diffuseColor = pow (texture(diffuse, UV).rgb, vec3(gamma)); // manual correction
``` 

## Shadows
## Normal Mapping
## HDR
## Bloom
## Deferred Shading
## SSAO