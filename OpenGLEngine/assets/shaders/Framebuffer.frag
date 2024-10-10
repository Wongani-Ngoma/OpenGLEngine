#version 460 core

in vec2 texCoords;

out vec4 FragColor;

uniform sampler2D screenTexture;
uniform float gamma;    

vec4 effect1();
vec4 effect2();
vec4 blackAndWhite(vec4 frag);
vec4 inverted(vec4 frag);

void main() {
    vec4 fragment = vec4(texture(screenTexture, texCoords));

    //fragment = blackAndWhite(fragment);
    //fragment = inverted(fragment);
    //fragment = effect1();
    fragment = effect2();

    FragColor.rgb = pow(fragment.rgb, vec3(1.0 / gamma)); // gamma correction
}

vec4 effect1() {
    const float offset = 1.0 / 300.0;

    vec2 offsets[9] = vec2[](
        vec2(-offset, offset), // top-left
        vec2( 0.0f, offset), // top-center
        vec2( offset, offset), // top-right
        vec2(-offset, 0.0f), // center-left
        vec2( 0.0f, 0.0f), // center-center
        vec2( offset, 0.0f), // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f, -offset), // bottom-center
        vec2( offset, -offset) // bottom-right
    );

    float kernel[9] = float[](
        1.0 / 16, 2.0 / 16, 1.0,
        2.0 / 16, 4.0 / 16, 2.0,
        1.0 / 16, 2.0 / 16, 1.0
    );

    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++) {
        sampleTex[i] = vec3(texture(screenTexture, texCoords.st + offsets[i]));
    }

    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];

    return vec4(col, 1.0);

}

vec4 effect2() {
    const float offset = 1.0 / 300.0;

    vec2 offsets[9] = vec2[](
        vec2(-offset, offset), // top-left
        vec2( 0.0f, offset), // top-center
        vec2( offset, offset), // top-right
        vec2(-offset, 0.0f), // center-left
        vec2( 0.0f, 0.0f), // center-center
        vec2( offset, 0.0f), // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f, -offset), // bottom-center
        vec2( offset, -offset) // bottom-right
    );

    float kernel[9] = float[](
        -2,  2,  4,
        -1,  -4, -1,
         4,  2, -2
    );

    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++) {
        sampleTex[i] = vec3(texture(screenTexture, texCoords.st + offsets[i]));
    }

    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];

    return vec4(col, 1.0);

}

vec4 blackAndWhite(vec4 frag) {
    //  the human eye tends to be more sensitive to green colors and the least to blue
    float average = 0.2126 * frag.r + 0.7152 * frag.g + 0.0722 * frag.b; 
    return vec4(average, average, average, 1.0); 
}

vec4 inverted(vec4 frag) {
    return vec4(1.0) - frag; 
}