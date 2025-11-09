#version 330 core

out vec4 FragColor;
in vec2 TexCoord;


uniform bool uEnableChroma;
uniform float uChromaIntensity;

uniform bool uEnableInvert;
uniform bool uEnableGrayscale;
uniform bool uEnableDither;
uniform bool uEnableGlitch;
uniform bool uCorrectGamma;
uniform bool uEnableLowColor;
uniform float colorDepth;

uniform bool uEnableKernel;
uniform int uKernelType; // 0=blur, 1=sharpen, 2=boxBlur, 3=emboss


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

void main(){

    vec4 scene = texture(screenTexture, TexCoord);
    vec4 finalColor = scene;

    // -- Stylized post process
    if (true || gl_FragCoord.x < sin(uTime * 100.0) * 1920){
        if (uEnableInvert)
            finalColor = vec4(vec3(1.0) - finalColor.rgb, 1.0);
        
        if (uEnableGrayscale){
            float avg = dot(finalColor.rgb, vec3(0.2126, 0.7152, 0.0722));
            finalColor = vec4(avg, avg, avg, 1.0);
        }

        if (uEnableChroma) {
            vec2 uv = TexCoord;
            float a = uChromaIntensity;
            vec3 chroma;
            chroma.r = texture(screenTexture, uv + vec2(a * sin(uTime * 2.0), 0.0)).r;
            chroma.g = texture(screenTexture, uv).g;
            chroma.b = texture(screenTexture, uv - vec2(0.0, a * cos(uTime * 2.5))).b;
            finalColor = vec4(chroma, 1.0);
        }

        if (uEnableKernel) {
            vec3 sampleTex[9];
            float kernel[9];
            
            if (uKernelType == 0)      kernel = blurKernel;
            else if (uKernelType == 1) kernel = sharpenKernel;
            else if (uKernelType == 2) kernel = boxBlur;
            else if (uKernelType == 3) kernel = embossKernel;

            vec3 color = vec3(0.0);
            for (int i = 0; i < 9; i++) {
                sampleTex[i] = texture(screenTexture, TexCoord + offsets[i]).rgb;
                color += sampleTex[i] * kernel[i];
            }
            
            finalColor = mix(finalColor, vec4(color, 1.0), .5);
        }

         // === DITHERING 4x4 ===
        if (uEnableDither) {
            float dither4x4[16] = float[16](
            0.0,  8.0,  2.0, 10.0,
            12.0, 4.0, 14.0, 6.0,
            3.0, 11.0, 1.0, 9.0,
            15.0, 7.0, 13.0, 5.0
            );
            int xi = int(mod(gl_FragCoord.x, 4.0));
            int yi = int(mod(gl_FragCoord.y, 4.0));
            float threshold = (dither4x4[yi * 4 + xi] + 0.5) / 16.0;
            finalColor.rgb = floor(finalColor.rgb + threshold);
        }

        if (uEnableLowColor){
            vec3 quantized = floor(finalColor.rgb * colorDepth) / colorDepth;
            finalColor.rgb = quantized;
        }

        // === GLITCH FX ===
        if (uEnableGlitch) {
            vec2 uv = TexCoord;
            float glitch = step(0.95, fract(sin(dot(uv * uTime, vec2(12.9898, 78.233))) * 43758.5453));
            if (glitch > 0.0) {
                uv.x += sin(uv.y * 1000.0 + uTime * 50.0) * 0.02;
            }
            finalColor = texture(screenTexture, uv);
        }
    }


    float gamma = 1.2;
    FragColor = finalColor;
    if (uCorrectGamma){
        FragColor.rgb = pow(FragColor.rgb, vec3(1.0/gamma)); // Gamma Correction in one line (flag it)
    }
}
