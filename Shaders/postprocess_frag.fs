#version 330 core

out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D screenTexture;
uniform float uTime;

const float offset = 1.0 / 300.0; // -- Try differents values
vec2 offsets[9] = vec2[](
vec2(-offset,  offset), // top-left
vec2( 0.0f,    offset), // top-center
vec2( offset,  offset), // top-right
vec2(-offset,  0.0f),   // center-left
vec2( 0.0f,    0.0f),   // center-center
vec2( offset,  0.0f),   // center-right
vec2(-offset, -offset), // bottom-left
vec2( 0.0f,   -offset), // bottom-center
vec2( offset, -offset)  // bottom-right    
);

// -- Could be uniform 
float sharpenKernel[9] = float[](
    -1, -1, -1,
    -1,  9, -1,
    -1, -1, -1
);

float blurKernel[9] = float[](
    1.0 / 16, 2.0 / 16, 1.0 / 16,
    2.0 / 16, 4.0 / 16, 2.0 / 16,
    1.0 / 16, 2.0 / 16, 1.0 / 16  
);

float boxBlur[9] = float[](
    1.0/9, 1.0/9, 1.0/9,
    1.0/9, 1.0/9, 1.0/9,
    1.0/9, 1.0/9, 1.0/9
);

float embossKernel[9] = float[](
   -2, -1, 0,
   -1,  1, 1,
    0,  1, 2
);

vec4 DrawUvs () { return vec4(TexCoord, 0.0, 1.0);}
vec4 InvertImage() { return vec4(vec3(1 - texture(screenTexture, TexCoord)), 1.0);}
vec4 ToGrayScale(){
    vec4 col = texture(screenTexture, TexCoord);
    float average = 0.2126 * col.r + 0.7152 * col.g + 0.0722 * col.b;
    return vec4(average, average, average, 1.0);
}

void main(){
    FragColor = DrawUvs();
    FragColor = InvertImage();
    FragColor = ToGrayScale();

    vec3 sampleTex[9];
    vec3 col = vec3(0.0);

    float kernel[9] = sharpenKernel;

    vec2 uv = TexCoord;
    float amount = 0.009;
    col.r = texture(screenTexture, uv + vec2(amount * sin(uTime * 2.0) * 5.0, 0.0)).r;
    col.g = texture(screenTexture, uv).g;
    col.b = texture(screenTexture, uv - vec2(0.0, amount * cos(uTime * 2.5) * 20.0)).b;
    FragColor = vec4(col, 1.0);

    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(screenTexture, TexCoord.xy + offsets[i]));
        col += sampleTex[i] * kernel[i];
    }

    FragColor = vec4(col, 1.0);
}
